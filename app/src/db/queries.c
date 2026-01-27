// Generated from SQL - do not edit

#include <stdlib.h>
#include <string.h>
#include "queries.h"

// GetCompetitor :one
int get_competitor(sqlite3 *db, sql_int64 id, void (*cb)(Competitor*, void*), void *ctx) {
    const char *sql = "select *\n"
                      "from competitor\n"
                      "where id = ?;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        Competitor result = {0};
        result.id = sqlite3_column_int64(stmt, 0);
        result.competitor_no.data = (sql_byte*)sqlite3_column_text(stmt, 1);
        result.competitor_no.len = sqlite3_column_bytes(stmt, 1);
        result.first_name.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.first_name.len = sqlite3_column_bytes(stmt, 2);
        result.last_name.data = (sql_byte*)sqlite3_column_text(stmt, 3);
        result.last_name.len = sqlite3_column_bytes(stmt, 3);
        result.email.data = (sql_byte*)sqlite3_column_text(stmt, 4);
        result.email.len = sqlite3_column_bytes(stmt, 4);
        result.mobile.data = (sql_byte*)sqlite3_column_text(stmt, 5);
        result.mobile.len = sqlite3_column_bytes(stmt, 5);
        result.phone.data = (sql_byte*)sqlite3_column_text(stmt, 6);
        result.phone.len = sqlite3_column_bytes(stmt, 6);
        result.address1.data = (sql_byte*)sqlite3_column_text(stmt, 7);
        result.address1.len = sqlite3_column_bytes(stmt, 7);
        result.address2.data = (sql_byte*)sqlite3_column_text(stmt, 8);
        result.address2.len = sqlite3_column_bytes(stmt, 8);
        result.suburb.data = (sql_byte*)sqlite3_column_text(stmt, 9);
        result.suburb.len = sqlite3_column_bytes(stmt, 9);
        result.state.data = (sql_byte*)sqlite3_column_text(stmt, 10);
        result.state.len = sqlite3_column_bytes(stmt, 10);
        result.postcode.data = (sql_byte*)sqlite3_column_text(stmt, 11);
        result.postcode.len = sqlite3_column_bytes(stmt, 11);
        if (sqlite3_column_type(stmt, 12) != SQLITE_NULL) {
            result.boat_id.value = sqlite3_column_int64(stmt, 12);
            result.boat_id.null = false;
        } else { result.boat_id.null = true; }
        cb(&result, ctx);
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

// ListCompetitors :many
int list_competitors(sqlite3 *db, void (*cb)(Competitor*, void*), void *ctx) {
    const char *sql = "select *\n"
                      "from competitor;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    size_t capacity = 16;
    size_t n = 0;
    Competitor *arr = malloc(capacity * sizeof(Competitor));
    if (arr == NULL) { sqlite3_finalize(stmt); return SQLITE_NOMEM; }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        Competitor result = {0};
        result.id = sqlite3_column_int64(stmt, 0);
        result.competitor_no.data = (sql_byte*)sqlite3_column_text(stmt, 1);
        result.competitor_no.len = sqlite3_column_bytes(stmt, 1);
        result.first_name.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.first_name.len = sqlite3_column_bytes(stmt, 2);
        result.last_name.data = (sql_byte*)sqlite3_column_text(stmt, 3);
        result.last_name.len = sqlite3_column_bytes(stmt, 3);
        result.email.data = (sql_byte*)sqlite3_column_text(stmt, 4);
        result.email.len = sqlite3_column_bytes(stmt, 4);
        result.mobile.data = (sql_byte*)sqlite3_column_text(stmt, 5);
        result.mobile.len = sqlite3_column_bytes(stmt, 5);
        result.phone.data = (sql_byte*)sqlite3_column_text(stmt, 6);
        result.phone.len = sqlite3_column_bytes(stmt, 6);
        result.address1.data = (sql_byte*)sqlite3_column_text(stmt, 7);
        result.address1.len = sqlite3_column_bytes(stmt, 7);
        result.address2.data = (sql_byte*)sqlite3_column_text(stmt, 8);
        result.address2.len = sqlite3_column_bytes(stmt, 8);
        result.suburb.data = (sql_byte*)sqlite3_column_text(stmt, 9);
        result.suburb.len = sqlite3_column_bytes(stmt, 9);
        result.state.data = (sql_byte*)sqlite3_column_text(stmt, 10);
        result.state.len = sqlite3_column_bytes(stmt, 10);
        result.postcode.data = (sql_byte*)sqlite3_column_text(stmt, 11);
        result.postcode.len = sqlite3_column_bytes(stmt, 11);
        if (sqlite3_column_type(stmt, 12) != SQLITE_NULL) {
            result.boat_id.value = sqlite3_column_int64(stmt, 12);
            result.boat_id.null = false;
        } else { result.boat_id.null = true; }
        cb(&result, ctx);
    }

    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) rc = SQLITE_OK;
    return rc;
}

// CreateCompetitor :one
int create_competitor(sqlite3 *db, CreateCompetitorParams *params, void (*cb)(Competitor*, void*), void *ctx) {
    const char *sql = "insert into competitor (\n"
                      "    first_name,\n"
                      "    last_name,\n"
                      "    email\n"
                      ") values ( ?, ?, ?)\n"
                      "returning id, first_name, last_name;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_text(stmt, 1, (char*)params->first_name.data, params->first_name.len, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, (char*)params->last_name.data, params->last_name.len, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, (char*)params->email.data, params->email.len, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        Competitor result = {0};
        result.id = sqlite3_column_int64(stmt, 0);
        result.first_name.data = (sql_byte*)sqlite3_column_text(stmt, 1);
        result.first_name.len = sqlite3_column_bytes(stmt, 1);
        result.last_name.data = (sql_byte*)sqlite3_column_text(stmt, 2);
        result.last_name.len = sqlite3_column_bytes(stmt, 2);
        cb(&result, ctx);
        rc = SQLITE_OK;
    } else if (rc == SQLITE_DONE) {
        rc = SQLITE_NOTFOUND;
    }
    sqlite3_finalize(stmt);
    return rc;
}

// DeleteCompetitor :exec
int delete_competitor(sqlite3 *db, sql_int64 id) {
    const char *sql = "delete from competitor\n"
                      "where id = ?;\n";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int64(stmt, 1, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? SQLITE_OK : rc;
}

