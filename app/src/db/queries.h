// Generated from SQL - do not edit

#ifndef QUERIES_H
#define QUERIES_H

#include "sqlite3.h"

#include "models.h"

// ============ Param Structs ============

typedef struct {
    sql_text first_name;
    sql_text last_name;
    sql_text email;
} CreateCompetitorParams;

typedef struct {
    sql_text first_name;
    sql_text last_name;
    sql_int64 id;
} UpdateCompetitorParams;

// ============ Query Functions ============

// select * from competitor where id = ?;
int get_competitor(sqlite3 *db, sql_int64 id, void (*cb)(Competitor*, void*), void *ctx);
// select * from competitor;
int list_competitors(sqlite3 *db, void (*cb)(Competitor*, void*), void *ctx);
// insert into competitor ( first_name, last_name, email ) values ( ?, ?, ?) returning id, first_name, last_name;
int create_competitor(sqlite3 *db, CreateCompetitorParams *params, void (*cb)(Competitor*, void*), void *ctx);
// update competitor set first_name = ?, last_name = ? where id = ? returning *;
int update_competitor(sqlite3 *db, UpdateCompetitorParams *params, void (*cb)(Competitor*, void*), void *ctx);
// delete from competitor where id = ?;
int delete_competitor(sqlite3 *db, sql_int64 id);


#endif // QUERIES_H
