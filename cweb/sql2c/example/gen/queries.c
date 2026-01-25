// Generated from SQL - do not edit

#include <stdlib.h>
#include <string.h>
#include "queries.h"

// Helper to allocate and copy text
static sql_text dup_text(sql_context *ctx, const unsigned char *src) {
    sql_text t = {0};
    if (src == NULL) return t;
    size_t len = strlen((const char *)src);
    if (ctx->alloc) {
        t.data = ctx->alloc(ctx, len + 1);
    } else {
        t.data = malloc(len + 1);
    }
    if (t.data) {
        memcpy(t.data, src, len + 1);
        t.len = len;
    }
    return t;
}

int get_competitor(sql_context *ctx, sql_int64 Id, Competitor *result) {
    const char *sql = "select * from competitor where id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, Id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        memset(result, 0, sizeof(*result));
        result->Id = sqlite3_column_int64(stmt, 0);
        result->FirstName = dup_text(ctx, sqlite3_column_text(stmt, 1));
        result->LastName = dup_text(ctx, sqlite3_column_text(stmt, 2));
        result->Email = dup_text(ctx, sqlite3_column_text(stmt, 3));
        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL) {
            result->BoatId.value = sqlite3_column_int64(stmt, 4);
            result->BoatId.null = false;
        } else { result->BoatId.null = true; }
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

int list_competitors(sql_context *ctx, Competitor **result, size_t *count) {
    const char *sql = "select * from competitor;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    size_t capacity = 16;
    size_t n = 0;
    Competitor *arr = malloc(capacity * sizeof(Competitor));
    if (arr == NULL) { sqlite3_finalize(stmt); return SQLITE_NOMEM; }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (n >= capacity) {
            capacity *= 2;
            Competitor *tmp = realloc(arr, capacity * sizeof(Competitor));
            if (tmp == NULL) { free(arr); sqlite3_finalize(stmt); return SQLITE_NOMEM; }
            arr = tmp;
        }
        Competitor *result = &arr[n++];
        memset(result, 0, sizeof(*result));
        result->Id = sqlite3_column_int64(stmt, 0);
        result->FirstName = dup_text(ctx, sqlite3_column_text(stmt, 1));
        result->LastName = dup_text(ctx, sqlite3_column_text(stmt, 2));
        result->Email = dup_text(ctx, sqlite3_column_text(stmt, 3));
        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL) {
            result->BoatId.value = sqlite3_column_int64(stmt, 4);
            result->BoatId.null = false;
        } else { result->BoatId.null = true; }
    }

    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) rc = SQLITE_OK;
    *result = arr;
    *count = n;
    return rc;
}

int create_competitor(sql_context *ctx, CreateCompetitorParams *params, Competitor *result) {
    const char *sql = "insert into competitor (     first_name,     last_name,     email ) values (?, ?, ?) returning *;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_text(stmt, 1, params->FirstName.data, params->FirstName.len, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, params->LastName.data, params->LastName.len, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, params->Email.data, params->Email.len, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        memset(result, 0, sizeof(*result));
        result->Id = sqlite3_column_int64(stmt, 0);
        result->FirstName = dup_text(ctx, sqlite3_column_text(stmt, 1));
        result->LastName = dup_text(ctx, sqlite3_column_text(stmt, 2));
        result->Email = dup_text(ctx, sqlite3_column_text(stmt, 3));
        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL) {
            result->BoatId.value = sqlite3_column_int64(stmt, 4);
            result->BoatId.null = false;
        } else { result->BoatId.null = true; }
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

int update_competitor_email(sql_context *ctx, UpdateCompetitorEmailParams *params, Competitor *result) {
    const char *sql = "update competitor set email = ? where id = ? returning *;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_text(stmt, 1, params->Email.data, params->Email.len, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, params->Id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        memset(result, 0, sizeof(*result));
        result->Id = sqlite3_column_int64(stmt, 0);
        result->FirstName = dup_text(ctx, sqlite3_column_text(stmt, 1));
        result->LastName = dup_text(ctx, sqlite3_column_text(stmt, 2));
        result->Email = dup_text(ctx, sqlite3_column_text(stmt, 3));
        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL) {
            result->BoatId.value = sqlite3_column_int64(stmt, 4);
            result->BoatId.null = false;
        } else { result->BoatId.null = true; }
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

int delete_competitor(sql_context *ctx, sql_int64 Id) {
    const char *sql = "delete from competitor where id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, Id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? SQLITE_OK : rc;
}

int get_boat(sql_context *ctx, sql_int64 Id, Boat *result) {
    const char *sql = "select * from boat where id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, Id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        memset(result, 0, sizeof(*result));
        result->Id = sqlite3_column_int64(stmt, 0);
        result->Name = dup_text(ctx, sqlite3_column_text(stmt, 1));
        result->Registration = dup_text(ctx, sqlite3_column_text(stmt, 2));
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

int list_boats(sql_context *ctx, Boat **result, size_t *count) {
    const char *sql = "select * from boat;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    size_t capacity = 16;
    size_t n = 0;
    Boat *arr = malloc(capacity * sizeof(Boat));
    if (arr == NULL) { sqlite3_finalize(stmt); return SQLITE_NOMEM; }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (n >= capacity) {
            capacity *= 2;
            Boat *tmp = realloc(arr, capacity * sizeof(Boat));
            if (tmp == NULL) { free(arr); sqlite3_finalize(stmt); return SQLITE_NOMEM; }
            arr = tmp;
        }
        Boat *result = &arr[n++];
        memset(result, 0, sizeof(*result));
        result->Id = sqlite3_column_int64(stmt, 0);
        result->Name = dup_text(ctx, sqlite3_column_text(stmt, 1));
        result->Registration = dup_text(ctx, sqlite3_column_text(stmt, 2));
    }

    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) rc = SQLITE_OK;
    *result = arr;
    *count = n;
    return rc;
}

int create_boat(sql_context *ctx, CreateBoatParams *params, Boat *result) {
    const char *sql = "insert into boat (name, registration) values (?, ?) returning *;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_text(stmt, 1, params->Name.data, params->Name.len, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, params->Registration.data, params->Registration.len, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        memset(result, 0, sizeof(*result));
        result->Id = sqlite3_column_int64(stmt, 0);
        result->Name = dup_text(ctx, sqlite3_column_text(stmt, 1));
        result->Registration = dup_text(ctx, sqlite3_column_text(stmt, 2));
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

int get_catch(sql_context *ctx, sql_int64 Id, Catch *result) {
    const char *sql = "select * from catch where id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, Id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        memset(result, 0, sizeof(*result));
        result->Id = sqlite3_column_int64(stmt, 0);
        result->CompetitorId = sqlite3_column_int64(stmt, 1);
        result->Species = dup_text(ctx, sqlite3_column_text(stmt, 2));
        result->WeightGrams = sqlite3_column_int64(stmt, 3);
        result->CaughtAt = dup_text(ctx, sqlite3_column_text(stmt, 4));
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

int list_catches_by_competitor(sql_context *ctx, sql_int64 CompetitorId, Catch **result, size_t *count) {
    const char *sql = "select * from catch where competitor_id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, CompetitorId);

    size_t capacity = 16;
    size_t n = 0;
    Catch *arr = malloc(capacity * sizeof(Catch));
    if (arr == NULL) { sqlite3_finalize(stmt); return SQLITE_NOMEM; }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (n >= capacity) {
            capacity *= 2;
            Catch *tmp = realloc(arr, capacity * sizeof(Catch));
            if (tmp == NULL) { free(arr); sqlite3_finalize(stmt); return SQLITE_NOMEM; }
            arr = tmp;
        }
        Catch *result = &arr[n++];
        memset(result, 0, sizeof(*result));
        result->Id = sqlite3_column_int64(stmt, 0);
        result->CompetitorId = sqlite3_column_int64(stmt, 1);
        result->Species = dup_text(ctx, sqlite3_column_text(stmt, 2));
        result->WeightGrams = sqlite3_column_int64(stmt, 3);
        result->CaughtAt = dup_text(ctx, sqlite3_column_text(stmt, 4));
    }

    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) rc = SQLITE_OK;
    *result = arr;
    *count = n;
    return rc;
}

int create_catch(sql_context *ctx, CreateCatchParams *params, Catch *result) {
    const char *sql = "insert into catch (competitor_id, species, weight_grams, caught_at) values (?, ?, ?, ?) returning *;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, params->CompetitorId);
    sqlite3_bind_text(stmt, 2, params->Species.data, params->Species.len, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 3, params->WeightGrams);
    sqlite3_bind_text(stmt, 4, params->CaughtAt.data, params->CaughtAt.len, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        memset(result, 0, sizeof(*result));
        result->Id = sqlite3_column_int64(stmt, 0);
        result->CompetitorId = sqlite3_column_int64(stmt, 1);
        result->Species = dup_text(ctx, sqlite3_column_text(stmt, 2));
        result->WeightGrams = sqlite3_column_int64(stmt, 3);
        result->CaughtAt = dup_text(ctx, sqlite3_column_text(stmt, 4));
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

