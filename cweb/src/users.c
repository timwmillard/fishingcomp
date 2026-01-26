#include <stdlib.h>

#include "ecewo.h"
#include "sqlite3.h"
#include "cJSON.h"
#include "slog.h"

// #include "cweb.h"
extern sqlite3 *sqldb;

// Context for get_all_users
typedef struct {
    Res *res;
    cJSON *json_array;
    int status;
    char *error_message;
} GetUsersContext;

// Worker function - runs in thread pool
void get_users_work(void *context) {
    // slog_debug("Get Users");
    printf("[DEGUG] GET USERS\n");
    GetUsersContext *ctx = (GetUsersContext *)context;
    
    const char *sql = "SELECT * FROM users;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(sqldb, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        ctx->status = 500;
        ctx->error_message = "DB prepare failed";
        return;
    }

    ctx->json_array = cJSON_CreateArray();

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        const char *username = (const char *)sqlite3_column_text(stmt, 2);

        cJSON *user_json = cJSON_CreateObject();
        cJSON_AddNumberToObject(user_json, "id", id);
        cJSON_AddStringToObject(user_json, "name", name);
        cJSON_AddStringToObject(user_json, "username", username);

        cJSON_AddItemToArray(ctx->json_array, user_json);
    }

    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        ctx->status = 500;
        ctx->error_message = "DB step failed";
        cJSON_Delete(ctx->json_array);
        ctx->json_array = NULL;
        return;
    }

    ctx->status = 200;
}

// Callback - runs on main thread
void get_users_done(void *context) {
    GetUsersContext *ctx = (GetUsersContext *)context;

    if (ctx->status != 200 || !ctx->json_array) {
        send_text(ctx->res, ctx->status, ctx->error_message);
        return;
    }

    char *json_string = cJSON_PrintUnformatted(ctx->json_array);
    send_json(ctx->res, 200, json_string);

    cJSON_Delete(ctx->json_array);
    free(json_string);
}

// Handler - runs on main thread
void get_all_users(Req *req, Res *res) {
    GetUsersContext *ctx = arena_alloc(res->arena, sizeof(GetUsersContext));
    ctx->res = res;
    ctx->json_array = NULL;
    ctx->status = 500;
    ctx->error_message = "Unknown error";

    spawn(ctx, get_users_work, get_users_done);
}
