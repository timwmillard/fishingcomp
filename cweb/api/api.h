// Generated from OpenAPI spec - do not edit

#ifndef API_H
#define API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "ecewo.h"

// ============ Allocator ============

// Allocator interface - pass NULL to use malloc/free
typedef struct {
    void* (*alloc)(void *ctx, size_t size);
    void  (*free)(void *ctx, void *ptr);  // Can be NULL for arenas
    void *ctx;
} api_Allocator;

// Arena to Allocator adapter for ecewo
static inline void *api_arena_alloc_fn(void *ctx, size_t size) {
    return arena_alloc((Arena *)ctx, size);
}

static inline api_Allocator api_arena_allocator(Arena *arena) {
    return (api_Allocator){ .alloc = api_arena_alloc_fn, .free = NULL, .ctx = arena };
}

// ============ Enums ============

// Struct forward declarations
typedef struct api_Competitor api_Competitor;

// ============ Structs ============

// A competitor in the fishing competition
struct api_Competitor {
    // First name of the competitor
    char* first_name;
    // Unique identifier for the competitor
    int64_t id;
    // Last name of the competitor
    char* last_name;
};

// ============ JSON Functions ============

// JSON serialization for api_Competitor
char *api_competitor_to_json(api_Allocator *alloc, api_Competitor *obj);
int api_competitor_from_json(api_Allocator *alloc, const char *json, api_Competitor *obj);
void api_competitor_free(api_Allocator *alloc, api_Competitor *obj);

// ============ Handlers (ecewo) ============

// GET /competitors - List all competitors
void api_get_competitors(Req *req, Res *res);

// POST /competitors - Create a competitor
void api_create_competitor(Req *req, Res *res);

// GET /competitors/{id} - Get a competitor by ID
void api_get_competitor(Req *req, Res *res);

// PUT /competitors/{id} - Update a competitor
void api_update_competitor(Req *req, Res *res);

// DELETE /competitors/{id} - Delete a competitor
void api_delete_competitor(Req *req, Res *res);


// ============ Routes (ecewo) ============

static inline void api_routes() {
    get("/competitors", api_get_competitors);    // List all competitors
    post("/competitors", api_create_competitor);    // Create a competitor
    get("/competitors/:id", api_get_competitor);    // Get a competitor by ID
    put("/competitors/:id", api_update_competitor);    // Update a competitor
    del("/competitors/:id", api_delete_competitor);    // Delete a competitor
}

#endif // API_H
