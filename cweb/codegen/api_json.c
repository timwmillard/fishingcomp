// Generated from OpenAPI spec - do not edit
// No external JSON library required

#include "api.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============ JSON Helper Functions ============

static const char *skip_ws(const char *p) {
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
    return p;
}

static const char *skip_value(const char *p) {
    p = skip_ws(p);
    if (*p == '"') {
        p++;
        while (*p && *p != '"') {
            if (*p == '\\' && *(p+1)) p++;
            p++;
        }
        if (*p == '"') p++;
    } else if (*p == '{') {
        int depth = 1;
        p++;
        while (*p && depth > 0) {
            if (*p == '{') depth++;
            else if (*p == '}') depth--;
            else if (*p == '"') {
                p++;
                while (*p && *p != '"') {
                    if (*p == '\\' && *(p+1)) p++;
                    p++;
                }
            }
            if (*p) p++;
        }
    } else if (*p == '[') {
        int depth = 1;
        p++;
        while (*p && depth > 0) {
            if (*p == '[') depth++;
            else if (*p == ']') depth--;
            else if (*p == '"') {
                p++;
                while (*p && *p != '"') {
                    if (*p == '\\' && *(p+1)) p++;
                    p++;
                }
            }
            if (*p) p++;
        }
    } else {
        while (*p && *p != ',' && *p != '}' && *p != ']') p++;
    }
    return p;
}

static const char *find_key(const char *json, const char *key) {
    const char *p = skip_ws(json);
    if (*p != '{') return NULL;
    p = skip_ws(p + 1);

    size_t keylen = strlen(key);
    while (*p && *p != '}') {
        if (*p != '"') return NULL;
        p++;
        const char *kstart = p;
        while (*p && *p != '"') p++;
        size_t klen = p - kstart;
        p++;
        p = skip_ws(p);
        if (*p != ':') return NULL;
        p = skip_ws(p + 1);

        if (klen == keylen && memcmp(kstart, key, keylen) == 0) {
            return p;
        }
        p = skip_value(p);
        p = skip_ws(p);
        if (*p == ',') p = skip_ws(p + 1);
    }
    return NULL;
}

static char *parse_string(const char *p, const char **endp) {
    p = skip_ws(p);
    if (*p != '"') { *endp = p; return NULL; }
    p++;
    const char *start = p;
    size_t len = 0;
    while (*p && *p != '"') {
        if (*p == '\\' && *(p+1)) { p += 2; len++; }
        else { p++; len++; }
    }
    char *str = malloc(len + 1);
    if (!str) { *endp = p; return NULL; }
    const char *s = start;
    char *d = str;
    while (s < p) {
        if (*s == '\\' && *(s+1)) {
            s++;
            switch (*s) {
                case 'n': *d++ = '\n'; break;
                case 't': *d++ = '\t'; break;
                case 'r': *d++ = '\r'; break;
                case '"': *d++ = '"'; break;
                case '\\': *d++ = '\\'; break;
                default: *d++ = *s; break;
            }
            s++;
        } else {
            *d++ = *s++;
        }
    }
    *d = '\0';
    *endp = (*p == '"') ? p + 1 : p;
    return str;
}

static int64_t parse_int(const char *p, const char **endp) {
    p = skip_ws(p);
    char *end;
    int64_t val = strtoll(p, &end, 10);
    *endp = end;
    return val;
}

static double parse_double(const char *p, const char **endp) {
    p = skip_ws(p);
    char *end;
    double val = strtod(p, &end);
    *endp = end;
    return val;
}

static bool parse_bool(const char *p, const char **endp) {
    p = skip_ws(p);
    if (strncmp(p, "true", 4) == 0) { *endp = p + 4; return true; }
    if (strncmp(p, "false", 5) == 0) { *endp = p + 5; return false; }
    *endp = p;
    return false;
}

static size_t count_array(const char *p) {
    p = skip_ws(p);
    if (*p != '[') return 0;
    p = skip_ws(p + 1);
    if (*p == ']') return 0;
    size_t count = 1;
    while (*p && *p != ']') {
        if (*p == ',' ) count++;
        else if (*p == '"') {
            p++;
            while (*p && *p != '"') {
                if (*p == '\\' && *(p+1)) p++;
                p++;
            }
        } else if (*p == '{' || *p == '[') {
            p = skip_value(p) - 1;
        }
        if (*p) p++;
    }
    return count;
}

static const char *array_first(const char *p) {
    p = skip_ws(p);
    if (*p != '[') return NULL;
    p = skip_ws(p + 1);
    if (*p == ']') return NULL;
    return p;
}

static const char *array_next(const char *p) {
    p = skip_value(p);
    p = skip_ws(p);
    if (*p == ',') return skip_ws(p + 1);
    return NULL;
}

static int write_escaped(char *buf, size_t size, const char *str) {
    if (!str) return snprintf(buf, size, "null");
    int len = 0;
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\"");
    for (const char *p = str; *p; p++) {
        switch (*p) {
            case '"':  len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\\\""); break;
            case '\\': len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\\\\"); break;
            case '\n': len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\\n"); break;
            case '\r': len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\\r"); break;
            case '\t': len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\\t"); break;
            default:   len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "%c", *p); break;
        }
    }
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\"");
    return len;
}

// Forward declarations
static const char *boat_parse(const char *json, Boat *obj);
static int boat_write(char *buf, size_t size, Boat *obj);
static const char *catch_parse(const char *json, Catch *obj);
static int catch_write(char *buf, size_t size, Catch *obj);
static const char *competitor_parse(const char *json, Competitor *obj);
static int competitor_write(char *buf, size_t size, Competitor *obj);
static const char *leaderboard_entry_parse(const char *json, LeaderboardEntry *obj);
static int leaderboard_entry_write(char *buf, size_t size, LeaderboardEntry *obj);
static const char *species_parse(const char *json, Species *obj);
static int species_write(char *buf, size_t size, Species *obj);

static int boat_write(char *buf, size_t size, Boat *obj) {
    if (obj == NULL) return snprintf(buf, size, "null");
    int len = 0;
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "{");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\"crew\":[");
    for (size_t i = 0; i < obj->crew_count; i++) {
        if (i > 0) len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",");
        len += competitor_write(buf + len, size > (size_t)len ? size - len : 0, &obj->crew[i]);
    }
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "]");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"id\":%lld", (long long)obj->id);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"name\":");
    len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->name);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"registration\":");
    len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->registration);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "}");
    return len;
}

char *boat_to_json(Boat *obj) {
    int len = boat_write(NULL, 0, obj);
    char *buf = malloc(len + 1);
    if (buf == NULL) return NULL;
    boat_write(buf, len + 1, obj);
    return buf;
}

static const char *boat_parse(const char *json, Boat *obj) {
    if (json == NULL || obj == NULL) return NULL;
    memset(obj, 0, sizeof(*obj));
    const char *p;
    (void)p; // may be unused

    p = find_key(json, "crew");
    if (p != NULL) {
        size_t count = count_array(p);
        obj->crew_count = count;
        if (count > 0) {
            obj->crew = calloc(count, sizeof(Competitor));
            const char *elem = array_first(p);
            for (size_t i = 0; i < count && elem; i++) {
                competitor_parse(elem, &obj->crew[i]);
                elem = array_next(elem);
            }
        }
    }

    p = find_key(json, "id");
    if (p != NULL) {
        obj->id = parse_int(p, &p);
    }

    p = find_key(json, "name");
    if (p != NULL) {
        obj->name = parse_string(p, &p);
    }

    p = find_key(json, "registration");
    if (p != NULL) {
        obj->registration = parse_string(p, &p);
    }

    return skip_value(json);
}

int boat_from_json(const char *json, Boat *obj) {
    return boat_parse(json, obj) != NULL ? 0 : -1;
}

void boat_free(Boat *obj) {
    if (obj == NULL) return;
    for (size_t i = 0; i < obj->crew_count; i++) competitor_free(&obj->crew[i]);
    free(obj->crew);
    free(obj->name);
    free(obj->registration);
}

static int catch_write(char *buf, size_t size, Catch *obj) {
    if (obj == NULL) return snprintf(buf, size, "null");
    int len = 0;
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "{");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\"caught_at\":");
    len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->caught_at);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"competitor_id\":%lld", (long long)obj->competitor_id);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"id\":%lld", (long long)obj->id);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"length_cm\":%g", (double)obj->length_cm);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"photos\":[");
    for (size_t i = 0; i < obj->photos_count; i++) {
        if (i > 0) len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",");
        len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->photos[i]);
    }
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "]");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"points\":%g", (double)obj->points);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"species\":");
    len += species_write(buf + len, size > (size_t)len ? size - len : 0, obj->species);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"verified\":%s", obj->verified ? "true" : "false");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"weight_kg\":%g", (double)obj->weight_kg);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "}");
    return len;
}

char *catch_to_json(Catch *obj) {
    int len = catch_write(NULL, 0, obj);
    char *buf = malloc(len + 1);
    if (buf == NULL) return NULL;
    catch_write(buf, len + 1, obj);
    return buf;
}

static const char *catch_parse(const char *json, Catch *obj) {
    if (json == NULL || obj == NULL) return NULL;
    memset(obj, 0, sizeof(*obj));
    const char *p;
    (void)p; // may be unused

    p = find_key(json, "caught_at");
    if (p != NULL) {
        obj->caught_at = parse_string(p, &p);
    }

    p = find_key(json, "competitor_id");
    if (p != NULL) {
        obj->competitor_id = parse_int(p, &p);
    }

    p = find_key(json, "id");
    if (p != NULL) {
        obj->id = parse_int(p, &p);
    }

    p = find_key(json, "length_cm");
    if (p != NULL) {
        obj->length_cm = (float)parse_double(p, &p);
    }

    p = find_key(json, "photos");
    if (p != NULL) {
        size_t count = count_array(p);
        obj->photos_count = count;
        if (count > 0) {
            obj->photos = calloc(count, sizeof(char*));
            const char *elem = array_first(p);
            for (size_t i = 0; i < count && elem; i++) {
                obj->photos[i] = parse_string(elem, &elem);
                elem = array_next(elem);
            }
        }
    }

    p = find_key(json, "points");
    if (p != NULL) {
        obj->points = parse_double(p, &p);
    }

    p = find_key(json, "species");
    if (p != NULL) {
        obj->species = calloc(1, sizeof(Species));
        if (obj->species) species_parse(p, obj->species);
    }

    p = find_key(json, "verified");
    if (p != NULL) {
        obj->verified = parse_bool(p, &p);
    }

    p = find_key(json, "weight_kg");
    if (p != NULL) {
        obj->weight_kg = parse_double(p, &p);
    }

    return skip_value(json);
}

int catch_from_json(const char *json, Catch *obj) {
    return catch_parse(json, obj) != NULL ? 0 : -1;
}

void catch_free(Catch *obj) {
    if (obj == NULL) return;
    free(obj->caught_at);
    for (size_t i = 0; i < obj->photos_count; i++) free(obj->photos[i]);
    free(obj->photos);
    if (obj->species) { species_free(obj->species); free(obj->species); }
}

static int competitor_write(char *buf, size_t size, Competitor *obj) {
    if (obj == NULL) return snprintf(buf, size, "null");
    int len = 0;
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "{");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\"boat\":");
    len += boat_write(buf + len, size > (size_t)len ? size - len : 0, obj->boat);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"catches\":[");
    for (size_t i = 0; i < obj->catches_count; i++) {
        if (i > 0) len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",");
        len += catch_write(buf + len, size > (size_t)len ? size - len : 0, &obj->catches[i]);
    }
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "]");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"created_at\":");
    len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->created_at);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"id\":%lld", (long long)obj->id);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"name\":");
    len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->name);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"total_points\":%g", (double)obj->total_points);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "}");
    return len;
}

char *competitor_to_json(Competitor *obj) {
    int len = competitor_write(NULL, 0, obj);
    char *buf = malloc(len + 1);
    if (buf == NULL) return NULL;
    competitor_write(buf, len + 1, obj);
    return buf;
}

static const char *competitor_parse(const char *json, Competitor *obj) {
    if (json == NULL || obj == NULL) return NULL;
    memset(obj, 0, sizeof(*obj));
    const char *p;
    (void)p; // may be unused

    p = find_key(json, "boat");
    if (p != NULL) {
        obj->boat = calloc(1, sizeof(Boat));
        if (obj->boat) boat_parse(p, obj->boat);
    }

    p = find_key(json, "catches");
    if (p != NULL) {
        size_t count = count_array(p);
        obj->catches_count = count;
        if (count > 0) {
            obj->catches = calloc(count, sizeof(Catch));
            const char *elem = array_first(p);
            for (size_t i = 0; i < count && elem; i++) {
                catch_parse(elem, &obj->catches[i]);
                elem = array_next(elem);
            }
        }
    }

    p = find_key(json, "created_at");
    if (p != NULL) {
        obj->created_at = parse_string(p, &p);
    }

    p = find_key(json, "id");
    if (p != NULL) {
        obj->id = parse_int(p, &p);
    }

    p = find_key(json, "name");
    if (p != NULL) {
        obj->name = parse_string(p, &p);
    }

    p = find_key(json, "total_points");
    if (p != NULL) {
        obj->total_points = parse_double(p, &p);
    }

    return skip_value(json);
}

int competitor_from_json(const char *json, Competitor *obj) {
    return competitor_parse(json, obj) != NULL ? 0 : -1;
}

void competitor_free(Competitor *obj) {
    if (obj == NULL) return;
    if (obj->boat) { boat_free(obj->boat); free(obj->boat); }
    for (size_t i = 0; i < obj->catches_count; i++) catch_free(&obj->catches[i]);
    free(obj->catches);
    free(obj->created_at);
    free(obj->name);
}

static int leaderboard_entry_write(char *buf, size_t size, LeaderboardEntry *obj) {
    if (obj == NULL) return snprintf(buf, size, "null");
    int len = 0;
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "{");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\"competitor\":");
    len += competitor_write(buf + len, size > (size_t)len ? size - len : 0, obj->competitor);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"rank\":%lld", (long long)obj->rank);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"total_catches\":%lld", (long long)obj->total_catches);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"total_points\":%g", (double)obj->total_points);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"total_weight_kg\":%g", (double)obj->total_weight_kg);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "}");
    return len;
}

char *leaderboard_entry_to_json(LeaderboardEntry *obj) {
    int len = leaderboard_entry_write(NULL, 0, obj);
    char *buf = malloc(len + 1);
    if (buf == NULL) return NULL;
    leaderboard_entry_write(buf, len + 1, obj);
    return buf;
}

static const char *leaderboard_entry_parse(const char *json, LeaderboardEntry *obj) {
    if (json == NULL || obj == NULL) return NULL;
    memset(obj, 0, sizeof(*obj));
    const char *p;
    (void)p; // may be unused

    p = find_key(json, "competitor");
    if (p != NULL) {
        obj->competitor = calloc(1, sizeof(Competitor));
        if (obj->competitor) competitor_parse(p, obj->competitor);
    }

    p = find_key(json, "rank");
    if (p != NULL) {
        obj->rank = (int32_t)parse_int(p, &p);
    }

    p = find_key(json, "total_catches");
    if (p != NULL) {
        obj->total_catches = (int32_t)parse_int(p, &p);
    }

    p = find_key(json, "total_points");
    if (p != NULL) {
        obj->total_points = parse_double(p, &p);
    }

    p = find_key(json, "total_weight_kg");
    if (p != NULL) {
        obj->total_weight_kg = parse_double(p, &p);
    }

    return skip_value(json);
}

int leaderboard_entry_from_json(const char *json, LeaderboardEntry *obj) {
    return leaderboard_entry_parse(json, obj) != NULL ? 0 : -1;
}

void leaderboard_entry_free(LeaderboardEntry *obj) {
    if (obj == NULL) return;
    if (obj->competitor) { competitor_free(obj->competitor); free(obj->competitor); }
}

static int species_write(char *buf, size_t size, Species *obj) {
    if (obj == NULL) return snprintf(buf, size, "null");
    int len = 0;
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "{");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\"id\":%lld", (long long)obj->id);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"min_size_cm\":%g", (double)obj->min_size_cm);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"name\":");
    len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->name);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"points_per_kg\":%g", (double)obj->points_per_kg);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"scientific_name\":");
    len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->scientific_name);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"tags\":[");
    for (size_t i = 0; i < obj->tags_count; i++) {
        if (i > 0) len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",");
        len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->tags[i]);
    }
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "]");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "}");
    return len;
}

char *species_to_json(Species *obj) {
    int len = species_write(NULL, 0, obj);
    char *buf = malloc(len + 1);
    if (buf == NULL) return NULL;
    species_write(buf, len + 1, obj);
    return buf;
}

static const char *species_parse(const char *json, Species *obj) {
    if (json == NULL || obj == NULL) return NULL;
    memset(obj, 0, sizeof(*obj));
    const char *p;
    (void)p; // may be unused

    p = find_key(json, "id");
    if (p != NULL) {
        obj->id = parse_int(p, &p);
    }

    p = find_key(json, "min_size_cm");
    if (p != NULL) {
        obj->min_size_cm = (float)parse_double(p, &p);
    }

    p = find_key(json, "name");
    if (p != NULL) {
        obj->name = parse_string(p, &p);
    }

    p = find_key(json, "points_per_kg");
    if (p != NULL) {
        obj->points_per_kg = parse_double(p, &p);
    }

    p = find_key(json, "scientific_name");
    if (p != NULL) {
        obj->scientific_name = parse_string(p, &p);
    }

    p = find_key(json, "tags");
    if (p != NULL) {
        size_t count = count_array(p);
        obj->tags_count = count;
        if (count > 0) {
            obj->tags = calloc(count, sizeof(char*));
            const char *elem = array_first(p);
            for (size_t i = 0; i < count && elem; i++) {
                obj->tags[i] = parse_string(elem, &elem);
                elem = array_next(elem);
            }
        }
    }

    return skip_value(json);
}

int species_from_json(const char *json, Species *obj) {
    return species_parse(json, obj) != NULL ? 0 : -1;
}

void species_free(Species *obj) {
    if (obj == NULL) return;
    free(obj->name);
    free(obj->scientific_name);
    for (size_t i = 0; i < obj->tags_count; i++) free(obj->tags[i]);
    free(obj->tags);
}

