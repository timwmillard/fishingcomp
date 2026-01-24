#ifndef API_H
#define API_H

// Generated from OpenAPI spec - do not edit

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Forward declarations
typedef struct Req Req;
typedef struct Res Res;

// Struct forward declarations
typedef struct Boat Boat;
typedef struct Catch Catch;
typedef struct Competitor Competitor;
typedef struct LeaderboardEntry LeaderboardEntry;
typedef struct Species Species;

// ============ Structs ============

// A registered fishing boat
struct Boat {
    // List of crew members
    Competitor *crew;
    size_t crew_count;
    // Unique identifier for the boat
    int64_t id;
    // Name of the boat
    char* name;
    // Official registration number
    char* registration;
};

// A recorded catch in the competition
struct Catch {
    // Timestamp when the fish was caught
    char* caught_at;
    // ID of the competitor who made the catch
    int64_t competitor_id;
    // Unique identifier for the catch
    int64_t id;
    // Length of the catch in centimeters
    float length_cm;
    // URLs of photos of the catch
    char* *photos;
    size_t photos_count;
    // Points awarded for this catch
    double points;
    // A fish species that can be caught
    Species *species;
    // Whether the catch has been verified by officials
    bool verified;
    // Weight of the catch in kilograms
    double weight_kg;
};

// A competitor in the fishing competition
struct Competitor {
    // A registered fishing boat
    Boat *boat;
    // List of catches made by this competitor
    Catch *catches;
    size_t catches_count;
    // Timestamp when the competitor was registered
    char* created_at;
    // Unique identifier for the competitor
    int64_t id;
    // Full name of the competitor
    char* name;
    // Total points accumulated
    double total_points;
};

// An entry in the competition leaderboard
struct LeaderboardEntry {
    // A competitor in the fishing competition
    Competitor *competitor;
    // Current ranking position
    int32_t rank;
    // Total number of catches
    int32_t total_catches;
    // Total points accumulated
    double total_points;
    // Total weight of all catches
    double total_weight_kg;
};

// A fish species that can be caught
struct Species {
    // Unique identifier for the species
    int64_t id;
    // Minimum legal size in centimeters
    float min_size_cm;
    // Common name of the species
    char* name;
    // Points awarded per kilogram
    double points_per_kg;
    // Scientific name of the species
    char* scientific_name;
    // Classification tags for this species
    char* *tags;
    size_t tags_count;
};

// ============ Handlers ============

// GET /boats - List all boats
void get_boats(Req *req, Res *res);

// POST /boats - Create a boat
void create_boat(Req *req, Res *res);

// GET /boats/{id} - Get a boat by ID
void get_boat(Req *req, Res *res);

// GET /catches - List all catches
void get_catches(Req *req, Res *res);

// POST /catches - Record a catch
void create_catch(Req *req, Res *res);

// GET /competitors - List all competitors
void get_competitors(Req *req, Res *res);

// POST /competitors - Create a competitor
void create_competitor(Req *req, Res *res);

// GET /competitors/{id} - Get a competitor by ID
void get_competitor(Req *req, Res *res);

// PUT /competitors/{id} - Update a competitor
void update_competitor(Req *req, Res *res);

// DELETE /competitors/{id} - Delete a competitor
void delete_competitor(Req *req, Res *res);

// GET /leaderboard - Get competition leaderboard
void get_leaderboard(Req *req, Res *res);

// GET /species - List all species
void get_species(Req *req, Res *res);


#endif // API_H
