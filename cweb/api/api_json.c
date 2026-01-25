// Generated from OpenAPI spec - do not edit

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "api.h"

// ============ JSON Helper Functions ============

// Allocator helpers - use allocator if provided, otherwise malloc/free
static void *json_alloc(api_Allocator *alloc, size_t size) {
    if (alloc && alloc->alloc) return alloc->alloc(alloc->ctx, size);
    return malloc(size);
}

static void *json_calloc(api_Allocator *alloc, size_t count, size_t size) {
    size_t total = count * size;
    void *ptr = json_alloc(alloc, total);
    if (ptr) memset(ptr, 0, total);
    return ptr;
}

static void json_free(api_Allocator *alloc, void *ptr) {
    if (ptr == NULL) return;
    if (alloc && alloc->free) { alloc->free(alloc->ctx, ptr); return; }
    if (alloc && alloc->alloc) return; // Arena without free - don't call free()
    free(ptr);
}

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

static char *parse_string(api_Allocator *alloc, const char *p, const char **endp) {
    p = skip_ws(p);
    if (*p != '"') { *endp = p; return NULL; }
    p++;
    const char *start = p;
    size_t len = 0;
    while (*p && *p != '"') {
        if (*p == '\\' && *(p+1)) { p += 2; len++; }
        else { p++; len++; }
    }
    char *str = json_alloc(alloc, len + 1);
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
static const char *api_competitor_parse(api_Allocator *alloc, const char *json, api_Competitor *obj);
static int api_competitor_write(api_Allocator *alloc, char *buf, size_t size, api_Competitor *obj);
static const char *api_error_parse(api_Allocator *alloc, const char *json, api_Error *obj);
static int api_error_write(api_Allocator *alloc, char *buf, size_t size, api_Error *obj);

static int api_competitor_write(api_Allocator *alloc, char *buf, size_t size, api_Competitor *obj) {
    (void)alloc; // unused in write, but kept for consistent signature
    if (obj == NULL) return snprintf(buf, size, "null");
    int len = 0;
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "{");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\"first_name\":");
    len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->first_name);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"id\":%lld", (long long)obj->id);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, ",\"last_name\":");
    len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->last_name);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "}");
    return len;
}

char *api_competitor_to_json(api_Allocator *alloc, api_Competitor *obj) {
    int len = api_competitor_write(alloc, NULL, 0, obj);
    char *buf = json_alloc(alloc, len + 1);
    if (buf == NULL) return NULL;
    api_competitor_write(alloc, buf, len + 1, obj);
    return buf;
}

static const char *api_competitor_parse(api_Allocator *alloc, const char *json, api_Competitor *obj) {
    if (json == NULL || obj == NULL) return NULL;
    memset(obj, 0, sizeof(*obj));
    const char *p;
    (void)p; // may be unused

    p = find_key(json, "first_name");
    if (p != NULL) {
        obj->first_name = parse_string(alloc, p, &p);
    }

    p = find_key(json, "id");
    if (p != NULL) {
        obj->id = parse_int(p, &p);
    }

    p = find_key(json, "last_name");
    if (p != NULL) {
        obj->last_name = parse_string(alloc, p, &p);
    }

    return skip_value(json);
}

int api_competitor_from_json(api_Allocator *alloc, const char *json, api_Competitor *obj) {
    return api_competitor_parse(alloc, json, obj) != NULL ? 0 : -1;
}

void api_competitor_free(api_Allocator *alloc, api_Competitor *obj) {
    if (obj == NULL) return;
    json_free(alloc, obj->first_name);
    json_free(alloc, obj->last_name);
}

static int api_error_write(api_Allocator *alloc, char *buf, size_t size, api_Error *obj) {
    (void)alloc; // unused in write, but kept for consistent signature
    if (obj == NULL) return snprintf(buf, size, "null");
    int len = 0;
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "{");
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "\"error\":");
    len += write_escaped(buf + len, size > (size_t)len ? size - len : 0, obj->error);
    len += snprintf(buf + len, size > (size_t)len ? size - len : 0, "}");
    return len;
}

char *api_error_to_json(api_Allocator *alloc, api_Error *obj) {
    int len = api_error_write(alloc, NULL, 0, obj);
    char *buf = json_alloc(alloc, len + 1);
    if (buf == NULL) return NULL;
    api_error_write(alloc, buf, len + 1, obj);
    return buf;
}

static const char *api_error_parse(api_Allocator *alloc, const char *json, api_Error *obj) {
    if (json == NULL || obj == NULL) return NULL;
    memset(obj, 0, sizeof(*obj));
    const char *p;
    (void)p; // may be unused

    p = find_key(json, "error");
    if (p != NULL) {
        obj->error = parse_string(alloc, p, &p);
    }

    return skip_value(json);
}

int api_error_from_json(api_Allocator *alloc, const char *json, api_Error *obj) {
    return api_error_parse(alloc, json, obj) != NULL ? 0 : -1;
}

void api_error_free(api_Allocator *alloc, api_Error *obj) {
    if (obj == NULL) return;
    json_free(alloc, obj->error);
}

