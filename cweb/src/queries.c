
#include "models.h"
#include "sqlite3.h"

int sql_GetCompetitor(sql_context *ctx, sql_int64 id, Competitor *result) {

  const char *sql =
      "-- name: GetCompetitor :one\n"
      "select id, competitor_no, first_name, last_name, email, mobile, phone, "
      "address1, address2, suburb, state, postcode, boat_id\n"
      "from competitor\n"
      "where id = ?\n";

  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK)
    return rc;

  sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE)
    return rc;

  rc = sqlite3_finalize(stmt);
  if (rc != SQLITE_OK)
    return rc;

  return SQLITE_OK;
}
