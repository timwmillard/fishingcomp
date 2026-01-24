#include "models.h"

typedef struct {
  sql_text FirstName;
  sql_text LastName;
  sql_text Email;
} CreateCompetitorParams;

int sql_GetCompetitor(sql_context *ctx, sql_int64 id, Competitor *result);
int sql_GetCompetitor_cb(sql_context *ctx, sql_int64 id, void (*cb)(Competitor *, void*), void *userdata);
int sql_CreateCompetitor_cb(sql_context *ctx, CreateCompetitorParams *arg, void (*cb)(Competitor *, void*), void *userdata);
