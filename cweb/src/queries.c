
#include "models.h"
#include "sqlite3.h"

typedef struct {
  sql_text FirstName;
  sql_text LastName;
  sql_text Email;
} CreateCompetitorParams;

int sql_CreateCompetitor(sql_context *ctx, CreateCompetitorParams arg,
                         Competitor *result) {

  const char *sql =
      "-- name: CreateCompetitor :one\n"
      "insert into competitor (\n"
      "    first_name,\n"
      "    last_name,\n"
      "    email\n"
      ") values ( ?, ?, ?)\n"
      "returning id, competitor_no, first_name, last_name, email, mobile, "
      "phone, address1, address2, suburb, state, postcode, boat_id\n";

  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v3(ctx->db, sql, -1, 0, &stmt, 0);
  if (rc != SQLITE_OK)
    return rc;

  sqlite3_bind_text(stmt, 1, arg.FirstName.data, -1, SQLITE_STATIC);

  sqlite3_bind_text(stmt, 2, arg.LastName.data, -1, SQLITE_STATIC);

  sqlite3_bind_text(stmt, 3, arg.Email.data, -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    result->ID = sqlite3_column_int64(stmt, 1);
    result->CompetitorNo.data = (char *)sqlite3_column_text(stmt, 2);
    result->FirstName.data = (char *)sqlite3_column_text(stmt, 3);
    result->LastName.data = (char *)sqlite3_column_text(stmt, 4);
    result->Email.data = (char *)sqlite3_column_text(stmt, 5);
    result->Mobile.data = (char *)sqlite3_column_text(stmt, 6);
    result->Phone.data = (char *)sqlite3_column_text(stmt, 7);
    result->Address1.data = (char *)sqlite3_column_text(stmt, 8);
    result->Address2.data = (char *)sqlite3_column_text(stmt, 9);
    result->Suburb.data = (char *)sqlite3_column_text(stmt, 10);
    result->State.data = (char *)sqlite3_column_text(stmt, 11);
    result->Postcode.data = (char *)sqlite3_column_text(stmt, 12);
    // Unknow type sql_nullint64 for BoatID
    sqlite3_finalize(stmt);
    return SQLITE_OK;
  } else if (rc == SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return SQLITE_NOTFOUND;
  } else {
    sqlite3_finalize(stmt);
    return rc;
  }
}

int sql_GetCompetitor(sql_context *ctx, sql_int64 id, Competitor *result) {

  const char *sql =
      "-- name: GetCompetitor :one\n"
      "select id, competitor_no, first_name, last_name, email, mobile, phone, "
      "address1, address2, suburb, state, postcode, boat_id\n"
      "from competitor\n"
      "where id = ?\n";

  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v3(ctx->db, sql, -1, 0, &stmt, 0);
  if (rc != SQLITE_OK)
    return rc;

  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    result->ID = sqlite3_column_int64(stmt, 1);
    result->CompetitorNo.data = (char *)sqlite3_column_text(stmt, 2);
    result->FirstName.data = (char *)sqlite3_column_text(stmt, 3);
    result->LastName.data = (char *)sqlite3_column_text(stmt, 4);
    result->Email.data = (char *)sqlite3_column_text(stmt, 5);
    result->Mobile.data = (char *)sqlite3_column_text(stmt, 6);
    result->Phone.data = (char *)sqlite3_column_text(stmt, 7);
    result->Address1.data = (char *)sqlite3_column_text(stmt, 8);
    result->Address2.data = (char *)sqlite3_column_text(stmt, 9);
    result->Suburb.data = (char *)sqlite3_column_text(stmt, 10);
    result->State.data = (char *)sqlite3_column_text(stmt, 11);
    result->Postcode.data = (char *)sqlite3_column_text(stmt, 12);
    // Unknow type sql_nullint64 for BoatID
    sqlite3_finalize(stmt);
    return SQLITE_OK;
  } else if (rc == SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return SQLITE_NOTFOUND;
  } else {
    sqlite3_finalize(stmt);
    return rc;
  }
}

int sql_ListCompetitors(sql_context *ctx, Competitor **result,
                        int *result_count) {

  const char *sql =
      "-- name: ListCompetitors :many\n"
      "select id, competitor_no, first_name, last_name, email, mobile, phone, "
      "address1, address2, suburb, state, postcode, boat_id\n"
      "from competitor\n";

  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v3(ctx->db, sql, -1, 0, &stmt, 0);
  if (rc != SQLITE_OK)
    return rc;

  int row = 0;
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    result[row]->ID = sqlite3_column_int64(stmt, 1);
    result[row]->CompetitorNo.data = (char *)sqlite3_column_text(stmt, 2);
    result[row]->FirstName.data = (char *)sqlite3_column_text(stmt, 3);
    result[row]->LastName.data = (char *)sqlite3_column_text(stmt, 4);
    result[row]->Email.data = (char *)sqlite3_column_text(stmt, 5);
    result[row]->Mobile.data = (char *)sqlite3_column_text(stmt, 6);
    result[row]->Phone.data = (char *)sqlite3_column_text(stmt, 7);
    result[row]->Address1.data = (char *)sqlite3_column_text(stmt, 8);
    result[row]->Address2.data = (char *)sqlite3_column_text(stmt, 9);
    result[row]->Suburb.data = (char *)sqlite3_column_text(stmt, 10);
    result[row]->State.data = (char *)sqlite3_column_text(stmt, 11);
    result[row]->Postcode.data = (char *)sqlite3_column_text(stmt, 12);
    // Unknow type sql_nullint64 for BoatID
    row++
  }
  *result_count = row;
  rc = sqlite3_finalize(stmt);
  if (rc != SQLITE_OK)
    return rc;
  return SQLITE_OK;
}
