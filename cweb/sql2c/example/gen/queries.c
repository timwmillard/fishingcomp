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

// GetCompetitor :one
int get_competitor(sqlite3 *db, sql_int64 Id, void (*cb)(Competitor*, void*), void *ctx) {
    const char *sql = "select *\n"
                      "from competitor\n"
                      "where id = ?;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, Id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        Competitor result;
        result.Id = sqlite3_column_int64(stmt, 0);
        result.FirstName.data = (sql_byte*)sqlite3_column_text(stmt, 1);
        result.FirstName.len = sqlite3_column_bytes(stmt, 1);
        result.LastName.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.LastName.len = sqlite3_column_bytes(stmt, 2);
        result.Email.data = (sql_byte*)sqlite3_column_text(stmt, 3);
        result.Email.len = sqlite3_column_bytes(stmt, 3);
        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL) {
            result.BoatId.value = sqlite3_column_int64(stmt, 4);
            result.BoatId.null = false;
        } else { result.BoatId.null = true; }
        cb(&result, ctx);
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

// ListCompetitors :many
int list_competitors(sql_context *ctx, Competitor **result, size_t *count) {
    const char *sql = "select *\n"
                      "from competitor;\n";
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
        result.Id = sqlite3_column_int64(stmt, 0);
        result.FirstName.data = (sql_byte*)sqlite3_column_text(stmt, 1);
        result.FirstName.len = sqlite3_column_bytes(stmt, 1);
        result.LastName.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.LastName.len = sqlite3_column_bytes(stmt, 2);
        result.Email.data = (sql_byte*)sqlite3_column_text(stmt, 3);
        result.Email.len = sqlite3_column_bytes(stmt, 3);
        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL) {
            result.BoatId.value = sqlite3_column_int64(stmt, 4);
            result.BoatId.null = false;
        } else { result.BoatId.null = true; }
    }

    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) rc = SQLITE_OK;
    *result = arr;
    *count = n;
    return rc;
}

// CreateCompetitor :one
int create_competitor(sqlite3 *db, CreateCompetitorParams *params, void (*cb)(Competitor*, void*), void *ctx) {
    const char *sql = "insert into competitor (\n"
                      "    first_name,\n"
                      "    last_name,\n"
                      "    email\n"
                      ") values (?, ?, ?)\n"
                      "returning *;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_text(stmt, 1, (char*)params->FirstName.data, params->FirstName.len, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, (char*)params->LastName.data, params->LastName.len, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, (char*)params->Email.data, params->Email.len, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        Competitor result;
        result.Id = sqlite3_column_int64(stmt, 0);
        result.FirstName.data = (sql_byte*)sqlite3_column_text(stmt, 1);
        result.FirstName.len = sqlite3_column_bytes(stmt, 1);
        result.LastName.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.LastName.len = sqlite3_column_bytes(stmt, 2);
        result.Email.data = (sql_byte*)sqlite3_column_text(stmt, 3);
        result.Email.len = sqlite3_column_bytes(stmt, 3);
        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL) {
            result.BoatId.value = sqlite3_column_int64(stmt, 4);
            result.BoatId.null = false;
        } else { result.BoatId.null = true; }
        cb(&result, ctx);
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

// UpdateCompetitorEmail :one
int update_competitor_email(sqlite3 *db, UpdateCompetitorEmailParams *params, void (*cb)(Competitor*, void*), void *ctx) {
    const char *sql = "update competitor\n"
                      "set email = ?\n"
                      "where id = ?\n"
                      "returning *;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_text(stmt, 1, (char*)params->Email.data, params->Email.len, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, params->Id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        Competitor result;
        result.Id = sqlite3_column_int64(stmt, 0);
        result.FirstName.data = (sql_byte*)sqlite3_column_text(stmt, 1);
        result.FirstName.len = sqlite3_column_bytes(stmt, 1);
        result.LastName.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.LastName.len = sqlite3_column_bytes(stmt, 2);
        result.Email.data = (sql_byte*)sqlite3_column_text(stmt, 3);
        result.Email.len = sqlite3_column_bytes(stmt, 3);
        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL) {
            result.BoatId.value = sqlite3_column_int64(stmt, 4);
            result.BoatId.null = false;
        } else { result.BoatId.null = true; }
        cb(&result, ctx);
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

// DeleteCompetitor :exec
int delete_competitor(sql_context *ctx, sql_int64 Id) {
    const char *sql = "delete from competitor\n"
                      "where id = ?;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, Id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? SQLITE_OK : rc;
}

// GetBoat :one
int get_boat(sqlite3 *db, sql_int64 Id, void (*cb)(Boat*, void*), void *ctx) {
    const char *sql = "select *\n"
                      "from boat\n"
                      "where id = ?;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, Id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        Boat result;
        result.Id = sqlite3_column_int64(stmt, 0);
        result.Name.data = (sql_byte*)sqlite3_column_text(stmt, 1);
        result.Name.len = sqlite3_column_bytes(stmt, 1);
        result.Registration.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.Registration.len = sqlite3_column_bytes(stmt, 2);
        cb(&result, ctx);
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

// ListBoats :many
int list_boats(sql_context *ctx, Boat **result, size_t *count) {
    const char *sql = "select *\n"
                      "from boat;\n";
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
        result.Id = sqlite3_column_int64(stmt, 0);
        result.Name.data = (sql_byte*)sqlite3_column_text(stmt, 1);
        result.Name.len = sqlite3_column_bytes(stmt, 1);
        result.Registration.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.Registration.len = sqlite3_column_bytes(stmt, 2);
    }

    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) rc = SQLITE_OK;
    *result = arr;
    *count = n;
    return rc;
}

// CreateBoat :one
int create_boat(sqlite3 *db, CreateBoatParams *params, void (*cb)(Boat*, void*), void *ctx) {
    const char *sql = "insert into boat (name, registration)\n"
                      "values (?, ?)\n"
                      "returning *;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_text(stmt, 1, (char*)params->Name.data, params->Name.len, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, (char*)params->Registration.data, params->Registration.len, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        Boat result;
        result.Id = sqlite3_column_int64(stmt, 0);
        result.Name.data = (sql_byte*)sqlite3_column_text(stmt, 1);
        result.Name.len = sqlite3_column_bytes(stmt, 1);
        result.Registration.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.Registration.len = sqlite3_column_bytes(stmt, 2);
        cb(&result, ctx);
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

// GetCatch :one
int get_catch(sqlite3 *db, sql_int64 Id, void (*cb)(Catch*, void*), void *ctx) {
    const char *sql = "select *\n"
                      "from catch\n"
                      "where id = ?;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, Id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        Catch result;
        result.Id = sqlite3_column_int64(stmt, 0);
        result.CompetitorId = sqlite3_column_int64(stmt, 1);
        result.Species.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.Species.len = sqlite3_column_bytes(stmt, 2);
        result.WeightGrams = sqlite3_column_int64(stmt, 3);
        result.CaughtAt.data = (sql_byte*)sqlite3_column_text(stmt, 4);
        result.CaughtAt.len = sqlite3_column_bytes(stmt, 4);
        cb(&result, ctx);
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

// ListCatchesByCompetitor :many
int list_catches_by_competitor(sql_context *ctx, sql_int64 CompetitorId, Catch **result, size_t *count) {
    const char *sql = "select *\n"
                      "from catch\n"
                      "where competitor_id = ?;\n";
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
        result.Id = sqlite3_column_int64(stmt, 0);
        result.CompetitorId = sqlite3_column_int64(stmt, 1);
        result.Species.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.Species.len = sqlite3_column_bytes(stmt, 2);
        result.WeightGrams = sqlite3_column_int64(stmt, 3);
        result.CaughtAt.data = (sql_byte*)sqlite3_column_text(stmt, 4);
        result.CaughtAt.len = sqlite3_column_bytes(stmt, 4);
    }

    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) rc = SQLITE_OK;
    *result = arr;
    *count = n;
    return rc;
}

// CreateCatch :one
int create_catch(sqlite3 *db, CreateCatchParams *params, void (*cb)(Catch*, void*), void *ctx) {
    const char *sql = "insert into catch (competitor_id, species, weight_grams, caught_at)\n"
                      "values (?, ?, ?, ?)\n"
                      "returning *;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, params->CompetitorId);
    sqlite3_bind_text(stmt, 2, (char*)params->Species.data, params->Species.len, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 3, params->WeightGrams);
    sqlite3_bind_text(stmt, 4, (char*)params->CaughtAt.data, params->CaughtAt.len, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        Catch result;
        result.Id = sqlite3_column_int64(stmt, 0);
        result.CompetitorId = sqlite3_column_int64(stmt, 1);
        result.Species.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.Species.len = sqlite3_column_bytes(stmt, 2);
        result.WeightGrams = sqlite3_column_int64(stmt, 3);
        result.CaughtAt.data = (sql_byte*)sqlite3_column_text(stmt, 4);
        result.CaughtAt.len = sqlite3_column_bytes(stmt, 4);
        cb(&result, ctx);
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

