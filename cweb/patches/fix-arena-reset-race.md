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

The request arena was being used for both HTTP parsing and write buffers, but these have different lifetimes:

1. Request arrives, HTTP context allocated from arena
2. Response and `write_req_t` allocated from same arena
3. `uv_write()` called - write is now pending
4. Write callback fires, calls `arena_reset()`
5. But libuv may still reference the `uv_write_t`, or another write is still pending
6. Memory corruption occurs

### Evidence

Debugging with lldb showed corrupted `uv_write_t` struct:

```
(lldb) p *req
(uv_write_t) {
  cb = 0x0000000000000000      // Should be write_completion_cb
  bufs = 0xffffffff00000000    // Corrupted pointer
  nbufs = 4294967295           // Garbage value
}
```

## Solution

Borrow a separate arena from the pool for each write operation. This decouples write buffer lifetime from the request arena:

- **Request arena** - HTTP parsing, reset between requests
- **Write arena** - borrowed per-write via `arena_borrow()`, returned via `arena_return()` when write completes

### Changes

**`write_completion_cb`**:
```c
// Before
arena_reset(write_req->arena);

// After
arena_return(write_req->arena);
```

**`send_error`**:
- Reset passed arena immediately (it's the request arena)
- Borrow a separate `write_arena` for the write operation
- Use `write_arena` for response buffer and `write_req_t`
- Return `write_arena` on errors or in callback

**`reply`**:
- Build headers using request arena (temporary)
- Borrow `write_arena` for response buffer and `write_req_t`
- Copy response data to `write_arena`
- Reset request arena after copy (safe - data is in write_arena)
- Return `write_arena` on errors or in callback

## Files Modified

- `src/response.c`

## Testing

```bash
wrk -c 2000 -t 8 -d 30s http://localhost:9000/health
```

Before: Crashes within seconds with malloc errors.
After: Completes successfully with no memory errors.
