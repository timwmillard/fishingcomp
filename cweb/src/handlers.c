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

void get_log_level_handle(Req *req, Res *res) {
    slog_logger *logger = slog_get_default();
    slog_level min_level = logger->handler->min_level;
    const char *level_str = slog_level_string(min_level);

    if (strcmp(level_str, "????") == 0)
        slog_debug("Logger get level", slog_int("min_level", min_level));
    else
        slog_debug("Logger get level", slog_string("min_level", level_str));

    char *body = arena_sprintf(req->arena, 
            "{\"min_level\": \"%s\"}\n", 
            level_str);
    send_json(res, OK, body);
}

void set_log_level_handle(Req *req, Res *res) {
    slog_logger *logger = slog_get_default();

    const char *level = get_param(req, "level");
    slog_level min_level;
    if (strcmp(level, "all") == 0) min_level = -1000;
    else if (strcmp(level, "debug") == 0) min_level = SLOG_DEBUG;
    else if (strcmp(level, "info") == 0) min_level = SLOG_INFO;
    else if (strcmp(level, "warn") == 0) min_level = SLOG_WARN;
    else if (strcmp(level, "error") == 0) min_level = SLOG_ERROR;
    else if (strcmp(level, "off") == 0) min_level = 1000;
    else send_json(res, BAD_REQUEST, "{\"error\": \"invalid log level\"}\n");

    const char *level_str = slog_level_string(min_level);
    if (strcmp(level_str, "????") == 0)
        slog_debug("Logger get level", slog_int("min_level", min_level));
    else
        slog_debug("Logger get level", slog_string("min_level", level_str));

    logger->handler->min_level = min_level;

    char *body = arena_sprintf(req->arena, 
            "{\"min_level\": \"%s\"}\n", level_str);
    send_json(res, OK, body);
}

void log_handle(Req *req, Res *res) {
    slog_debug("Debugging log",
            slog_bool("status", true)
    );
    slog_info("Information log",
            slog_bool("status", true)
    );
    slog_warn("Warning log",
            slog_bool("status", true)
    );
    slog_error("Error log",
            slog_bool("status", true)
    );
    reply(res, OK, NULL, 0);
}


// ------------ API Spec Implementation ---------------
typedef struct {
    Req *req;
    Res *res;
    api_Allocator alloc;
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
}

void competitor_to_json(Competitor *comp, void *data) {
    context *ctx = data;
    ctx->body = api_competitor_to_json(&ctx->alloc, &(api_Competitor){
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

void create_competitor_work(void *cx) {
    context *ctx = cx;
    Req *req = ctx->req;
    Res *res = ctx->res;

    api_Competitor body;
    int rc = api_competitor_from_json(&ctx->alloc, req->body, &body);
    if (rc != 0) {
        slog_error("Competitor bad request");
        response(ctx, BAD_REQUEST, "{\"error\": \"malformed json\"}\n");
        return;
    }

    ctx->log_message = "Create Competitor";
    rc = create_competitor(sqldb, &(CreateCompetitorParams){
            .first_name = to_sql_text(body.first_name),
            .last_name = to_sql_text(body.last_name),
            .email = to_sql_text(""),
        }, competitor_to_json, ctx);
    if (rc != SQLITE_OK) {
        slog_error("Competitor create failed");
        response(ctx, INTERNAL_SERVER_ERROR, api_error_to_json(&ctx->alloc, 
                    &(api_Error){.error = "Competitor not created, error occured"}));
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
    context *ctx = arena_alloc(req->arena, sizeof(context));
    memset(ctx, 0, sizeof(context));
    ctx->req = req;
    ctx->res = res;
    ctx->alloc = api_arena_allocator(req->arena);
    spawn(ctx, create_competitor_work, work_done_send_json);
}

void get_competitor_work(void *cx) {
    context *ctx = cx;

    const char *id_str = get_param(ctx->req, "id");
    int id = atoi(id_str);

    ctx->log_message = "Get Competitor";
    int rc = get_competitor(sqldb, id, competitor_to_json, ctx);
    if (rc != SQLITE_OK) {
        slog_error("Competitor not found",
                slog_int("id", id)
        );
        response(ctx, NOT_FOUND, api_error_to_json(&ctx->alloc, 
                    &(api_Error){.error = "Competitor not found"}));
        return;
    }
}

// GET /competitors/{id} - Get a competitor by ID
void api_get_competitor(Req *req, Res *res) {
    context *ctx = arena_alloc(req->arena, sizeof(context));
    memset(ctx, 0, sizeof(context));
    ctx->req = req;
    ctx->res = res;
    ctx->alloc = api_arena_allocator(req->arena);
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



void routes() {
    get("/health", health);
    get("/log", log_handle);
    get("/log/level", get_log_level_handle);
    post("/log/level/:level", set_log_level_handle);

    api_routes();
}
