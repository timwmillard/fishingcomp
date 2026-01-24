// Generated from OpenAPI spec - do not edit

#ifndef API_H
#define API_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// // Forward declarations
// typedef struct Req Req;
// typedef struct Res Res;

// Struct forward declarations
typedef struct Competitor api_Competitor;

// ============ Structs ============

// A competitor in the fishing competition
struct Competitor {
    // First name of the competitor
    char* first_name;
    // Unique identifier for the competitor
    int64_t id;
    // Last name of the competitor
    char* last_name;
};

// ============ JSON Functions ============

// JSON serialization for Competitor
char *competitor_to_json(api_Competitor *obj);
int competitor_from_json(const char *json, api_Competitor *obj);
void competitor_free(api_Competitor *obj);

// ============ Handlers ============

// // GET /competitors - List all competitors
// void get_competitors(Req *req, Res *res);
//
// // POST /competitors - Create a competitor
// void create_competitor(Req *req, Res *res);
//
// // GET /competitors/{id} - Get a competitor by ID
// void get_competitor(Req *req, Res *res);
//
// // PUT /competitors/{id} - Update a competitor
// void update_competitor(Req *req, Res *res);
//
// // DELETE /competitors/{id} - Delete a competitor
// void delete_competitor(Req *req, Res *res);


#endif // API_H
