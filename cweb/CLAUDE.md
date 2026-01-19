# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build and Run Commands

**Build:**
```bash
cmake --build build
```

**Run:**
```bash
./build/app -port=9000 -log=json
```

**Development with live reload:**
```bash
air
```
Uses Air to watch files and automatically rebuild on changes.

**Initial CMake setup (first time only):**
```bash
cmake -B build
```

## Architecture

This is a C web server for a fishing competition management system.

**Tech stack:**
- C with CMake build system
- ecewo web framework (HTTP routing/serving)
- SQLite embedded database (amalgamation)
- slog.h for structured logging (JSON or text output)
- flag.h for CLI argument parsing

**Key files:**
- `src/main.c` - Entry point, database init, server startup
- `src/handlers.c` - HTTP route handlers (included directly in main.c)
- `src/cweb.h` - Project constants and global db connection
- `sql/schema.sql` - Database schema (embedded into binary via bin2c)
- `src/schema.h` - Auto-generated from schema.sql during build

**Database:**
- SQLite database file: `fishingcomp.db`
- Schema is embedded in the binary and executed on startup
- Tables: boat, competitor, species, catch

**Build process:**
1. CMake fetches ecewo framework and SQLite amalgamation
2. bin2c tool generates `src/schema.h` from `sql/schema.sql`
3. Main app compiles with embedded schema

**Adding routes:**
Define handler functions in `src/handlers.c` and register them in `routes()` using `get()`, `post()`, etc. from ecewo.
