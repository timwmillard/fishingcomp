#include <stdlib.h>

#include "ecewo.h"
#include "slog.h"
#include "cweb.h"

#include "models.h"
#include "queries.h"
#include "api.h"


void health(Req *req, Res *res) {
    // int rc = sqlite3_db_readonly(sqlctx.db, "main");
    int rc = sqlite3_exec(sqldb, "SELECT 1", NULL, NULL, NULL);
    // res->keep_alive = 0;  // Disable keep-alive
    
    char *body = arena_sprintf(req->arena, 
            "{\n"
            "  \"version\": \"" VERSION "\",\n"
            "  \"db_connected\": %s\n"
            "}\n", rc==SQLITE_OK?"true":"false");
    send_json(res, OK, body);
}

// ------------ API Spec Implementation ---------------
typedef struct {
    Req *req;
    Res *res;
    Arena *arena;
    api_Allocator pool_alloc;
    int status;
    char *body;
    char *log_message;
} context;

static void response(context *ctx, int status, char *body) {
    ctx->status = status;
    ctx->body = body;
}

void work_done_send_json(void *cx) {
    context *ctx = cx;
    send_json(ctx->res, ctx->status, ctx->body);
    // arena_return(ctx->arena);
}

void competitor_to_json(Competitor *comp, void *data) {
    context *ctx = data;
    ctx->body = api_competitor_to_json(&ctx->pool_alloc, &(api_Competitor){
            .id = comp->id,
            .first_name = (char*)comp->first_name.data,
            .last_name = (char*)comp->last_name.data,
        });
    ctx->status = OK;

    if (ctx->log_message)
        slog_debug(ctx->log_message,
                slog_int("id", comp->id),
                slog_string("first_name", (char*)comp->first_name.data),
                slog_string("last_name", (char*)comp->last_name.data)
                );
}

void get_competitor_work(void *cx) {
    context *ctx = cx;

    // Main thread arena
    // ctx->pool_alloc = api_arena_allocator(ctx->req->arena);
    
    // New arena
    Arena *arena = arena_borrow();
    ctx->pool_alloc = api_arena_allocator(arena);

    const char *id_str = get_param(ctx->req, "id");
    int id = atoi(id_str);

    ctx->log_message = "Get Competitor";
    int rc = get_competitor(sqldb, id, competitor_to_json, ctx);
    if (rc != SQLITE_OK) {
        slog_error("Competitor not found",
                slog_int("id", id)
        );
        response(ctx, NOT_FOUND, api_error_to_json(&ctx->pool_alloc, 
                    &(api_Error){.error = "Competitor not found"}));
        return;
    }
}

// GET /competitors - List all competitors
void api_get_competitors(Req *req, Res *res) {
    slog_debug("Get Competitors Not Implmented");
    reply(res, NOT_IMPLEMENTED, NULL, 0);
}

// POST /competitors - Create a competitor
void api_create_competitor(Req *req, Res *res) {
    slog_debug("Create Competitors Not Implmented");
    reply(res, NOT_IMPLEMENTED, NULL, 0);
}


// GET /competitors/{id} - Get a competitor by ID
void api_get_competitor(Req *req, Res *res) {
    context *ctx = arena_alloc(req->arena, sizeof(context));
    memset(ctx, 0, sizeof(context));
    ctx->req = req;
    ctx->res = res;
    // ctx->alloc = api_arena_allocator(req->arena);
    spawn(ctx, get_competitor_work, work_done_send_json);
}

// PUT /competitors/{id} - Update a competitor
void api_update_competitor(Req *req, Res *res) {
    slog_debug("Update Competitors Not Implmented");
    reply(res, NOT_IMPLEMENTED, NULL, 0);
}

// DELETE /competitors/{id} - Delete a competitor
void api_delete_competitor(Req *req, Res *res) {
    slog_debug("Delete Competitors Not Implmented");
    reply(res, NOT_IMPLEMENTED, NULL, 0);
}

typedef struct {
    Arena *arena;
    char *data;
} Buffer;

void competitor_to_json_sync(Competitor *comp, void *data) {
    Buffer *buf = data;
    api_Allocator alloc = api_arena_allocator(buf->arena);
    buf->data = api_competitor_to_json(&alloc, &(api_Competitor){
            .id = comp->id,
            .first_name = (char*)comp->first_name.data,
            .last_name = (char*)comp->last_name.data,
        });
    slog_debug("Get Competitor",
            slog_int("id", comp->id),
            slog_string("first_name", (char*)comp->first_name.data),
            slog_string("last_name", (char*)comp->last_name.data)
        );
}

// GET /competitors/{id} - Get a competitor by ID
void api_get_competitor_sync(Req *req, Res *res) {
    api_Allocator alloc = api_arena_allocator(req->arena);
    const char *id_str = get_param(req, "id");
    int id = atoi(id_str);

    Buffer buf = {.arena = req->arena};
    int rc = get_competitor(sqldb, id, competitor_to_json_sync, &buf);
    if (rc != SQLITE_OK) {
        slog_error("Competitor not found",
                slog_int("id", id)
        );
        send_json(res, NOT_FOUND, api_error_to_json(&alloc, 
                    &(api_Error){.error = "Competitor not found"}));
        return;
    }
    send_json(res, OK, buf.data);
}


void get_log_level_handle(Req *req, Res *res);
void set_log_level_handle(Req *req, Res *res);
void log_handle(Req *req, Res *res);
void get_all_users(Req *req, Res *res);

void routes() {
    get("/health", health);
    get("/log", log_handle);
    get("/log/level", get_log_level_handle);
    post("/log/level/:level", set_log_level_handle);
    get("/users", get_all_users);

    get("/sync/competitors/:id", api_get_competitor_sync); // Get a competitor by ID

    api_routes();
}
