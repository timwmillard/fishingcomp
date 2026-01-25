// Generated from SQL - do not edit

#ifndef SQL_MODEL_H
#define SQL_MODEL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef unsigned char sql_byte;

typedef double sql_double;

typedef struct {
    sql_double value;
    bool null;
} sql_nulldouble;

typedef int sql_int;

typedef struct {
    sql_int value;
    bool null;
} sql_nullint;

typedef int64_t sql_int64;

typedef struct {
    sql_int64 value;
    bool null;
} sql_nullint64;

typedef double sql_numeric;

typedef struct {
    sql_numeric value;
    bool null;
} sql_nullnumeric;

typedef bool sql_bool;

typedef struct {
    sql_bool value;
    bool null;
} sql_nullbool;

typedef struct {
    sql_byte *data;
    size_t len;
} sql_blob;

typedef struct {
    sql_byte *data;
    size_t len;
    bool null;
} sql_nullblob;

typedef struct {
    sql_byte *data;
    size_t len;
} sql_text;

typedef struct {
    char *data;
    size_t len;
    bool null;
} sql_nulltext;

// ============ Table Structs ============

typedef struct {
    sql_int64 id;
    sql_text boat_no;
    sql_text name;
    sql_text boat_rego;
} Boat;

typedef struct {
    sql_int64 id;
    sql_int64 competitor_id;
    sql_int64 species_id;
    sql_int64 size;
    sql_text caught_at;
    sql_text bait;
    sql_text location_name;
    sql_nullint64 location;
} Catch;

typedef struct {
    sql_int64 id;
    sql_text competitor_no;
    sql_text first_name;
    sql_text last_name;
    sql_text email;
    sql_text mobile;
    sql_text phone;
    sql_text address1;
    sql_text address2;
    sql_text suburb;
    sql_text state;
    sql_text postcode;
    sql_nullint64 boat_id;
} Competitor;

typedef struct {
    sql_int64 id;
    sql_text name;
    sql_text slug;
    sql_text scientific_name;
    sql_text common_names;
    sql_text photo_url;
    sql_text type;
} Species;

#endif // SQL_MODEL_H
