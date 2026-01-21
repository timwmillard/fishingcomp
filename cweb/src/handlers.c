#include <stdlib.h>

#include "ecewo.h"
#include "slog.h"
#include "cweb.h"

#include "models.h"

int sql_GetCompetitor(sql_context *ctx, sql_int64 id, Competitor *result);

void health(Req *req, Res *res) {
    send_json(res, OK, "{\"version\": \"" VERSION "\"}\n");
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
            "  \"id\": %d,\n"
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
    get("/hello", hello);
    get("/users/:id", get_user);
    get("/v1/competitors/:id", get_competitor);
}
