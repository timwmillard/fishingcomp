#include <stdlib.h>

#include "ecewo.h"
#include "slog.h"
#include "cweb.h"

#include "models.h"

int sql_GetCompetitor(sql_context *ctx, sql_int64 id, Competitor *result);
int sql_GetCompetitor_cb(sql_context *ctx, sql_int64 id, void (*cb)(Competitor *, void*), void *userdata);

void health(Req *req, Res *res) {
    // res->keep_alive = 0;  // Disable keep-alive
    send_json(res, OK, "{\"version\": \"" VERSION "\"}\n");
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

void get_competitor(Req *req, Res *res) {
    const char *id_str = get_param(req, "id");
    int id = atoi(id_str);

    slog_debug("Competitor getting",
            slog_int("id", id)
    );

    Competitor comp;
    int rc = sql_GetCompetitor(&sqlctx, id, &comp);
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

void routes() {
    get("/health", health);
    get("/log", log_handle);
    get("/log/level", get_log_level_handle);
    post("/log/level/:level", set_log_level_handle);
    get("/hello", hello);
    get("/users/:id", get_user);
    get("/v1/competitors/:id", get_competitor);
    get("/v2/competitors/:id", get_competitor_v2);
}
