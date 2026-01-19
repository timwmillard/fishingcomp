

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef unsigned char sql_byte;

typedef double sql_double;
typedef struct {
  sql_double value;
  bool is_null;
} sql_nulldouble;

typedef int sql_int;
typedef struct {
  sql_int value;
  bool is_null;
} sql_nullint;

typedef int64_t sql_int64;
typedef struct {
  sql_int64 value;
  bool is_null;
} sql_nullint64;

typedef double sql_numeric;
typedef struct {
  sql_numeric value;
  bool is_null;
} sql_nullnumeric;

typedef bool sql_bool;
typedef struct {
  sql_bool value;
  bool is_null;
} sql_nullbool;

typedef struct {
  sql_byte *data;
  size_t len;
} sql_blob;
typedef struct {
  sql_byte *data;
  size_t len;
  bool is_null;
} sql_nullblob;

typedef struct {
  char *data;
  size_t len;
} sql_text;
typedef struct {
  char *data;
  size_t len;
  bool is_null;
} sql_nulltext;

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
  sql_text Firstname;
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
