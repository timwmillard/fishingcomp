#include <stdio.h>
#include <stdint.h>

#include "ecewo.h"
#include "sqlite3.h"
#include "slog.h"
#include "flag.h"

#include "cweb.h"
#include "schema.h"
#include "handlers.c"

void usage(FILE *stream)
{
    fprintf(stream, "Usage: ./example [OPTIONS] [--] [ARGS]\n");
    fprintf(stream, "OPTIONS:\n");
    flag_print_options(stream);
}

void handle_ecewo_log(LogLevel level, const char *file, int line,
                  const char *fmt, va_list args) {
    char msg[1024];
    vsnprintf(msg, sizeof(msg), fmt, args);

    slog_logger *logger = slog_get_default();
    slog_log_impl(logger, (slog_level)level, file, line, "",  msg,
            slog_bool("ecewo_log", true),
            #ifdef DEBUG
            slog_string("file", file),
            slog_int("line", line),
            #endif
            NULL);
}

void db_open(const char *db_name) {
    int rc = sqlite3_open(db_name, &sqldb);
    if (rc != SQLITE_OK) {
        slog_error("Database open failed",
                slog_string("error",  sqlite3_errmsg(sqldb))
        );
        sqlite3_close(sqldb);
        exit(1);
    }
    slog_info("Database opened",
            slog_string("database_name", db_name)
    );

    // Execute embedded schema
    char *err_msg = NULL;
    rc = sqlite3_exec(sqldb, (char*)sql_schema_sql_data, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        slog_error("Database schema execution failed",
                slog_string("error",  sqlite3_errmsg(sqldb))
        );
        sqlite3_free(err_msg);
        exit(1);
    }
    slog_info("Database schema exec",
            slog_string("database_name", db_name)
    );
}

int main(int argc, char *argv[]) {
    bool        *help = flag_bool("help", false, "Print this help to stdout and exit with 0");
    uint64_t    *port = flag_uint64("port", 8080, "server port number");
    char **log_format = flag_str("log", "text", "logging format");

    if (!flag_parse(argc, argv)) {
        usage(stderr);
        flag_print_error(stderr);
        exit(1);
    }

    argc = flag_rest_argc();
    argv = flag_rest_argv();
    if (*help) {
        usage(stdout);
        exit(0);
    }

    slog_handler *log_handler;
    slog_level level = SLOG_DEBUG;
    if (strcmp(*log_format, "json") == 0)
        log_handler = slog_json_handler_new(stdout, level);
    else if (strcmp(*log_format, "color") == 0 || strcmp(*log_format, "humanlog") == 0)
        log_handler = slog_color_text_handler_new(stdout, level);
    else 
        log_handler = slog_text_handler_new(stdout, level);

    slog_logger *logger = slog_new(log_handler);
    slog_set_default(logger);

    server_set_log_handler(handle_ecewo_log);
    server_set_log_level((LogLevel)level);

    slog_info("Logger started",
                 slog_string("min_log_level", slog_level_string(level))
                 );

    const char *db_name = "fishingcomp.db";
    if (argc > 0) db_name = argv[0]; 
    db_open(db_name);

    int err;
    int backlog = 0;

    slog_info("Server starting",
                 slog_string("version", VERSION),
                 slog_string("log_format", *log_format)
                 );

    if (server_init() != 0) {
        slog_error("Server failed to initialise");
        return -1;
    }

    routes();

    if (server_listen(*port) != 0) {
        slog_error("Server failed to listen on",
                slog_int("port", *port));
        return -1;
    }

    slog_info("Server listening",
             slog_int("port", *port)
             );

    server_run();
    return 0;
}
