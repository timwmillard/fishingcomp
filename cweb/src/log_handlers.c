#include "ecewo.h"
#include "slog.h"

#include "cweb.h"

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
