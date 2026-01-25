
#ifndef SQL_MODEL_H
#define SQL_MODEL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite3.h"

typedef unsigned char sql_byte;

typedef double sql_double;

typedef struct {
    sql_double value;
    bool null;
} sql_nulldouble;

typedef int sql_int;

typedef struct {
    sql_int value;
    bool null;
} sql_nullint;

typedef int64_t sql_int64;

typedef struct {
    sql_int64 value;
    bool null;
} sql_nullint64;

typedef double sql_numeric;

typedef struct {
    sql_numeric value;
    bool null;
} sql_nullnumeric;

typedef bool sql_bool;

typedef struct {
    sql_bool value;
    bool null;
} sql_nullbool;

typedef struct {
    sql_byte *data;
    size_t len;
} sql_blob;

typedef struct {
    sql_byte *data;
    size_t len;
    bool null;
} sql_nullblob;

typedef struct {
    sql_byte *data;
    size_t len;
} sql_text;

typedef struct {
    char *data;
    size_t len;
    bool null;
} sql_nulltext;

static inline sql_text to_sql_text(char *str) {
    return (sql_text){
        .data = (sql_byte*)str,
        .len = strlen(str)
    };
}

typedef struct {
    sqlite3 *db;
    void* (*alloc)(void *ctx, size_t size);
    void *(*realloc)(void *ctx, void *oldptr, size_t oldsz, size_t newsz);
    void  (*free)(void *ctx, void *ptr);  // Can be NULL for arenas
    void *ctx;
} sql_context;

// Allocator helpers - use allocator if provided, otherwise malloc/free
static inline void *sql_alloc(sql_context *ctx, size_t size) {
    if (ctx && ctx->alloc) return ctx->alloc(ctx->ctx, size);
    return malloc(size);
}

static inline void *sql_realloc(sql_context *ctx, void *oldptr, size_t oldsz, size_t newsz) {
    if (ctx && ctx->alloc) return ctx->realloc(ctx->ctx, oldptr, oldsz, newsz);
    return realloc(oldptr, newsz);
}

static inline void *sql_calloc(sql_context *ctx, size_t count, size_t size) {
    size_t total = count * size;
    void *ptr = sql_alloc(ctx, total);
    if (ptr) memset(ptr, 0, total);
    return ptr;
}

static inline void sql_free(sql_context *ctx, void *ptr) {
    if (ptr == NULL) return;
    if (ctx && ctx->free) { ctx->free(ctx->ctx, ptr); return; }
    if (ctx && ctx->alloc) return; // Arena without free - don't call free()
    free(ptr);
}

// Table structs

typedef struct {
    sql_int64 Id;
    sql_text Name;
    sql_text Registration;
} Boat;

typedef struct {
    sql_int64 Id;
    sql_text FirstName;
    sql_text LastName;
    sql_text Email;
    sql_nullint64 BoatId;
} Competitor;

typedef struct {
    sql_int64 Id;
    sql_int64 CompetitorId;
    sql_text Species;
    sql_int64 WeightGrams;
    sql_text CaughtAt;
} Catch;

#endif
