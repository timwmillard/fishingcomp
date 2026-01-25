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

typedef struct {
    sql_text Email;
    sql_int64 Id;
} UpdateCompetitorEmailParams;

typedef struct {
    sql_text Name;
    sql_text Registration;
} CreateBoatParams;

typedef struct {
    sql_int64 CompetitorId;
    sql_text Species;
    sql_int64 WeightGrams;
    sql_text CaughtAt;
} CreateCatchParams;

// ============ Query Functions ============

// select * from competitor where id = ?;
int get_competitor(sql_context *ctx, sql_int64 Id, Competitor *result);

// select * from competitor;
int list_competitors(sql_context *ctx, Competitor **result, size_t *count);

// insert into competitor ( first_name, last_name, email ) values (?, ?, ?) returning *;
int create_competitor(sql_context *ctx, CreateCompetitorParams *params, Competitor *result);

// update competitor set email = ? where id = ? returning *;
int update_competitor_email(sql_context *ctx, UpdateCompetitorEmailParams *params, Competitor *result);

// delete from competitor where id = ?;
int delete_competitor(sql_context *ctx, sql_int64 Id);

// select * from boat where id = ?;
int get_boat(sql_context *ctx, sql_int64 Id, Boat *result);

// select * from boat;
int list_boats(sql_context *ctx, Boat **result, size_t *count);

// insert into boat (name, registration) values (?, ?) returning *;
int create_boat(sql_context *ctx, CreateBoatParams *params, Boat *result);

// select * from catch where id = ?;
int get_catch(sql_context *ctx, sql_int64 Id, Catch *result);

// select * from catch where competitor_id = ?;
int list_catches_by_competitor(sql_context *ctx, sql_int64 CompetitorId, Catch **result, size_t *count);

// insert into catch (competitor_id, species, weight_grams, caught_at) values (?, ?, ?, ?) returning *;
int create_catch(sql_context *ctx, CreateCatchParams *params, Catch *result);

#endif // QUERIES_H
