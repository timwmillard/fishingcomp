# Fix: Arena Reset Race Condition with Pending Writes

## Summary

This patch fixes a race condition in the ecewo HTTP server where the connection
arena is reset while `uv_write` operations are still pending, causing memory
corruption and crashes under high concurrency with keep-alive connections.

## Problem

### Symptoms

Under high concurrency (e.g., `wrk -c 2000 -t 8`), the server crashes with:

```
malloc: *** error for object 0xffffffff00000000: pointer being freed was not allocated
```

The crash occurs in libuv's `uv__write_callbacks` when it tries to access the `uv_write_t` request struct after the write completes.

### Root Cause

The issue is a lifetime mismatch between arena-allocated write buffers and asynchronous write operations:

1. **Request 1** arrives on a keep-alive connection
2. Server allocates `write_req_t` and response buffer from the connection arena
3. `uv_write()` is called - the write is now **pending** in libuv's queue
4. The write callback fires and calls `arena_reset()`
5. **But**: libuv may still reference the `uv_write_t` struct after the callback returns
6. Or: another write from the same connection is still pending when `arena_reset()` is called
7. The reset invalidates memory that libuv is still using, causing corruption

### Evidence

Debugging with lldb showed the corrupted `uv_write_t` struct:

```
(lldb) p *req
(uv_write_t) {
  cb = 0x0000000000000000      // Should be write_completion_cb
  bufs = 0xffffffff00000000    // Corrupted pointer
  nbufs = 4294967295           // Garbage value
  ...
}
```

The `bufs` pointer `0xffffffff00000000` indicates memory that was overwritten after being freed/reset.

## Solution

The fix decouples write buffer lifetime from the arena by using `malloc` for all write operations instead of arena allocation.

### Key Changes

1. **Remove arena field from write_req_t** - no longer needed
2. **Always use malloc for write buffers** - response string and write_req_t struct
3. **Reset arena before the write** - after copying needed data to malloc'd buffer
4. **Free in callback** - simple free() of malloc'd buffers

### Why This Works

- The arena is reset immediately after building the response (before `uv_write`)
- Write buffers are independently allocated and freed
- No lifetime coupling between arena and pending writes
- The callback simply frees the malloc'd memory when the write completes

### Trade-off

This approach uses malloc per response instead of arena allocation. The performance impact is minimal because:
- Response buffers are typically small
- Modern allocators handle small allocations efficiently
- The arena is still used for HTTP parsing (method, URL, headers, body)
- Correctness is more important than micro-optimization

## Files Modified

- `src/response.c` - Use malloc for write buffers, reset arena before write

## Testing

The fix was verified with:

```bash
wrk -c 2000 -t 8 -d 30s http://localhost:9000/health
```

Before the fix: Crashes within seconds with malloc errors.
After the fix: Completes successfully with no memory errors.
