#ifndef SQL_MODEL_H
#define SQL_MODEL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "sqlite3.h"

typedef unsigned char sql_byte;

typedef double sql_double;

typedef struct {
  sql_double data;
  bool valid;
} sql_nulldouble;

typedef int sql_int;

typedef struct {
  sql_int data;
  bool valid;
} sql_nullint;

typedef int64_t sql_int64;

typedef struct {
  sql_int64 data;
  bool valid;
} sql_nullint64;

typedef double sql_numeric;

typedef struct {
  sql_numeric data;
  bool valid;
} sql_nullnumeric;

typedef bool sql_bool;

typedef struct {
  sql_bool data;
  bool valid;
} sql_nullbool;

typedef struct {
  sql_byte *data;
  size_t len;
} sql_blob;

typedef struct {
  sql_byte *data;
  size_t len;
  bool valid;
} sql_nullblob;

typedef struct {
  const sql_byte *data;
  size_t len;
} sql_text;

typedef struct {
  const sql_byte *data;
  size_t len;
  bool valid;
} sql_nulltext;

typedef struct {
  sqlite3 *db;
  void *(*alloc)(void *, int);
} sql_context;

typedef struct {
  sql_int64 ID;
  sql_text BoatNo;
  sql_text Name;
  sql_text BoatRego;
} Boat;

typedef struct {
  sql_int64 ID;
  sql_int64 CompetitorID;
  sql_int64 SpeciesID;
  sql_int64 Size;
  void *CaughtAt;
  sql_text Bait;
  sql_text LocationName;
  void *Location;
} Catch;

typedef struct {
  sql_int64 ID;
  sql_text CompetitorNo;
  sql_text FirstName;
  sql_text LastName;
  sql_text Email;
  sql_text Mobile;
  sql_text Phone;
  sql_text Address1;
  sql_text Address2;
  sql_text Suburb;
  sql_text State;
  sql_text Postcode;
  sql_nullint64 BoatID;
} Competitor;

typedef struct {
  sql_int64 ID;
  sql_text Name;
  sql_text Slug;
  sql_text ScientificName;
  sql_text CommonNames;
  sql_text PhotoUrl;
  sql_text Type;
} Species;

static inline sql_text to_sql_text(char *str) {
    return (sql_text){
        .data = (sql_byte*)str,
        .len = strlen(str)
    };
}

#endif
