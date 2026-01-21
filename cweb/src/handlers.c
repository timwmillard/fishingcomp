
#include "ecewo.h"
#include "slog.h"
#include "cweb.h"

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
}
