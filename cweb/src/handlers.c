#include <stdlib.h>

#include "ecewo.h"
#include "slog.h"
#include "cweb.h"

#include "models.h"
#include "queries.h"
#include "api.h"
#include "api_json.c"


void health(Req *req, Res *res) {
    // int rc = sqlite3_db_readonly(sqlctx.db, "main");
    int rc = sqlite3_exec(sqlctx.db, "SELECT 1", NULL, NULL, NULL);
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


void hello(Req *req, Res *res) {
    const char *name = get_query(req, "name");
    char *res_body = arena_sprintf(res->arena, "{\"name\": \"%s\"}\n", name);
    send_json(res, OK, res_body);
}

void get_user(Req *req, Res *res) {
    const char *id = get_param(req, "id");
    char *res_body = arena_sprintf(res->arena, "{\"id\": %s}\n", id);
    send_json(res, OK, res_body);
}

void get_competitor_res(Competitor *comp, void *data) {
    Res *res = data;
    char *body = arena_sprintf(res->arena, 
            "{\n"
            "  \"id\": %lld"",\n"
            "  \"first_name\": \"%s\",\n"
            "  \"last_name\": \"%s\"\n"
            "}\n",
            comp->ID, comp->FirstName.data, comp->LastName.data
    );
    send_json(res, OK, body);
}

void get_competitor_v2(Req *req, Res *res) {
    const char *id_str = get_param(req, "id");
    int id = atoi(id_str);

    slog_debug("Competitor getting",
            slog_int("id", id)
    );

    Competitor comp;
    int rc = sql_GetCompetitor_cb(&sqlctx, id, get_competitor_res, res);
    if (rc != SQLITE_OK) {
        slog_error("Competitor not found",
                slog_int("id", id)
        );
        send_json(res, NOT_FOUND, "{\"error\": \"competior not found\"}\n");
        return;
    }

    slog_info("Competitor get",
            slog_int("id", id)
    );
}

void get_competitor_v1(Req *req, Res *res) {
    const char *id_str = get_param(req, "id");
    int id = atoi(id_str);

    Competitor comp;
    int rc = sql_GetCompetitor(&sqlctx, id, &comp);
    switch (rc) {
        case SQLITE_OK: break;
        case SQLITE_NOTFOUND:
            slog_debug("Competitor not found",
                    slog_int("id", id),
                    slog_string("error", sqlite3_errstr(rc)),
                    slog_int("error_code", rc)
            );
            send_json(res, NOT_FOUND, "{\"error\": \"competior not found\"}\n");
            return;
        default:
            slog_error("Competitor get error",
                    slog_int("id", id),
                    slog_string("error", sqlite3_errstr(rc)),
                    slog_int("error_code", rc)
            );
            send_json(res, INTERNAL_SERVER_ERROR, "{\"error\": \"something when wrong\"}\n");
            return;
    }

    slog_debug("Competitor get",
            slog_int("id", id)
    );

    char *res_body = arena_sprintf(res->arena, 
            "{\n"
            "  \"id\": %lld,\n"
            "  \"first_name\": \"%s\",\n"
            "  \"last_name\": \"%s\"\n"
            "}\n",
            comp.ID, comp.FirstName.data, comp.LastName.data
    );
    send_json(res, OK, res_body);
}

void api_get_competitor_res(Competitor *comp, void *data) {
    Res *res = data;

    slog_info("Competitor get",
            slog_int("id", comp->ID)
    );
    char *body = competitor_to_json(&(api_Competitor){
            .id = comp->ID,
            .first_name = (char*)comp->FirstName.data,
            .last_name = (char*)comp->LastName.data,
        });
    send_json(res, OK, body);
}

void sql2api_competitor(Competitor *comp, void *ctx) {
    api_Competitor *api = ctx;
    api->id = comp->ID;
    api->first_name = strdup((char*)comp->FirstName.data);  // TODO: better without strdup??
    api->last_name = strdup((char*)comp->LastName.data);
}

// GET /competitors/{id} - Get a competitor by ID
void api_get_competitor(Req *req, Res *res) {
    const char *id_str = get_param(req, "id");
    int id = atoi(id_str);

    slog_debug("Competitor getting",
            slog_int("id", id)
    );

    Competitor comp;
    int rc = sql_GetCompetitor_cb(&sqlctx, id, api_get_competitor_res, res);
    if (rc != SQLITE_OK) {
        slog_error("Competitor not found",
                slog_int("id", id)
        );
        send_json(res, NOT_FOUND, "{\"error\": \"competior not found\"}\n");
        return;
    }
}

typedef struct {
    Req *req;
    Res *res;
    int status;
    char *body;
} context;

static void response(context *ctx, int status, char *body) {
    ctx->status = status;
    ctx->body = body;
}

// POST /competitors - Create a competitor
void api_create_competitor(void *cx) {
    context *ctx = cx;
    slog_debug("Competitor creating");
    Req *req = ctx->req;
    Res *res = ctx->res;

    api_Competitor body;
    int rc = competitor_from_json(req->body, &body);
    if (rc != 0) {
        slog_error("Competitor bad request");
        response(ctx, BAD_REQUEST, "{\"error\": \"malformed json\"}\n");
        return;
    }

    api_Competitor result;
    rc = sql_CreateCompetitor_cb(&sqlctx, &(CreateCompetitorParams){
            .FirstName = to_sql_text(body.first_name),
            .LastName = to_sql_text(body.last_name),
            .Email = to_sql_text(""),
        }, sql2api_competitor, &result);
    if (rc != SQLITE_OK) {
        slog_error("Competitor create failed");
        response(ctx, INTERNAL_SERVER_ERROR, "{\"error\": \"create competior failed\"}\n");
        return;
    }

    slog_info("Competitor created",
            slog_int("id", result.id),
            slog_string("first_name", result.first_name),
            slog_string("last_name", result.last_name)
            );

    response(ctx, OK, competitor_to_json(&result));
}

void work_done_send_json(void *cx) {
    context *ctx = cx;
    send_json(ctx->res, ctx->status, ctx->body);
}

void create_competitor_v3(Req *req, Res *res) {
    context *ctx = arena_alloc(req->arena, sizeof(context));
    ctx->req = req;
    ctx->res = res;
    spawn(ctx, api_create_competitor, work_done_send_json);
}

void routes() {
    get("/health", health);
    get("/log", log_handle);
    get("/log/level", get_log_level_handle);
    post("/log/level/:level", set_log_level_handle);
    get("/hello", hello);
    get("/users/:id", get_user);
    get("/v1/competitors/:id", get_competitor_v1);
    get("/v2/competitors/:id", get_competitor_v2);

    // API
    get("/api/competitors/:id", api_get_competitor);
    post("/api/competitors", api_create_competitor);

    // API v3
    post("/v3/competitors", create_competitor_v3);
}
