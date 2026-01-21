
#include "ecewo.h"
#include "slog.h"
#include "cweb.h"

void health(Req *req, Res *res) {
    send_json(res, OK, "{\"version\": \"" VERSION "\"}\n");
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
}
