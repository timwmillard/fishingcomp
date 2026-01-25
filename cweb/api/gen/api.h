// Generated from OpenAPI spec - do not edit

#ifndef API_H
#define API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Forward declarations
typedef struct Req Req;
typedef struct Res Res;

// ============ Enums ============

// Struct forward declarations
typedef struct api_Competitor api_Competitor;

// ============ Structs ============

// A competitor in the fishing competition
struct api_Competitor {
    // First name of the competitor
    char* first_name;
    // Unique identifier for the competitor
    int64_t id;
    // Last name of the competitor
    char* last_name;
};

// ============ JSON Functions ============

// JSON serialization for api_Competitor
char *api_competitor_to_json(api_Competitor *obj);
int api_competitor_from_json(const char *json, api_Competitor *obj);
void api_competitor_free(api_Competitor *obj);

// ============ Handlers ============

// GET /competitors - List all competitors
void api_get_competitors(Req *req, Res *res);

// POST /competitors - Create a competitor
void api_create_competitor(Req *req, Res *res);

// GET /competitors/{id} - Get a competitor by ID
void api_get_competitor(Req *req, Res *res);

// PUT /competitors/{id} - Update a competitor
void api_update_competitor(Req *req, Res *res);

// DELETE /competitors/{id} - Delete a competitor
void api_delete_competitor(Req *req, Res *res);


#endif // API_H
