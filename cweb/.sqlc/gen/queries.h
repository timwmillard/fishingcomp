// Generated from SQL - do not edit

#ifndef QUERIES_H
#define QUERIES_H

#include "models.h"

// ============ Param Structs ============

typedef struct {
    sql_text FirstName;
    sql_text LastName;
    sql_text Email;
} CreateCompetitorParams;

// ============ Query Functions ============

// select * from competitor where id = ?;
int get_competitor(sql_context *ctx, sql_int64 Id, Competitor *result);

// select * from competitor;
int list_competitors(sql_context *ctx, Competitor **result, size_t *count);

// insert into competitor ( first_name, last_name, email ) values ( ?, ?, ?) returning *;
int create_competitor(sql_context *ctx, CreateCompetitorParams *params, Competitor *result);

// delete from competitor where id = ?;
int delete_competitor(sql_context *ctx, sql_int64 Id);

#endif // QUERIES_H
