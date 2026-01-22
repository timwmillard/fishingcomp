cmake_minimum_required(VERSION 3.14)
project(app VERSION 1.0.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  ecewo
  GIT_REPOSITORY https://github.com/timwmillard/ecewo.git
  GIT_TAG working
  # GIT_TAG v3.1.0
  GIT_SHALLOW TRUE
  PATCH_COMMAND patch -p1 -N < ${CMAKE_SOURCE_DIR}/patches/fix-arena-reset-race.patch || true
)

file(DOWNLOAD
  https://raw.githubusercontent.com/tsoding/flag.h/refs/heads/master/flag.h
  ${CMAKE_SOURCE_DIR}/vendor/flag.h
)

# Download SQLite amalgamation to vendor folder
set(SQLITE_VERSION 3510200)
set(SQLITE_YEAR 2026)
set(SQLITE_URL "https://sqlite.org/${SQLITE_YEAR}/sqlite-amalgamation-${SQLITE_VERSION}.zip")
set(SQLITE_ARCHIVE "${CMAKE_BINARY_DIR}/sqlite-amalgamation-${SQLITE_VERSION}.zip")

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/vendor/sqlite3.c")
  file(DOWNLOAD ${SQLITE_URL} ${SQLITE_ARCHIVE})
  file(ARCHIVE_EXTRACT INPUT ${SQLITE_ARCHIVE} DESTINATION ${CMAKE_BINARY_DIR})
  file(COPY
    ${CMAKE_BINARY_DIR}/sqlite-amalgamation-${SQLITE_VERSION}/sqlite3.c
    ${CMAKE_BINARY_DIR}/sqlite-amalgamation-${SQLITE_VERSION}/sqlite3.h
    DESTINATION ${CMAKE_SOURCE_DIR}/vendor
  )
endif()

FetchContent_MakeAvailable(ecewo)

# Build bin2c tool for schema generation
add_executable(bin2c tool/bin2c.c)
set_target_properties(bin2c PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tool
)
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/src/schema.h
    COMMAND $<TARGET_FILE:bin2c> ${CMAKE_SOURCE_DIR}/sql/schema.sql ${CMAKE_SOURCE_DIR}/src/schema.h sql_schema_sql
    DEPENDS ${CMAKE_SOURCE_DIR}/sql/schema.sql bin2c
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating schema.h from sql/schema.sql using bin2c"
)
add_custom_target(generate_schema DEPENDS ${CMAKE_SOURCE_DIR}/src/schema.h)

# Custom targets
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/src/models.h
    COMMAND go tool sqlc generate
    COMMAND clang-format -i ${CMAKE_SOURCE_DIR}/sql/gen/models.h ${CMAKE_SOURCE_DIR}/sql/gen/queries.sql.c
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/sql/gen/models.h ${CMAKE_SOURCE_DIR}/src/models.h
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/sql/gen/queries.sql.c ${CMAKE_SOURCE_DIR}/src/queries.c
    DEPENDS ${CMAKE_SOURCE_DIR}/sql/schema.sql ${CMAKE_SOURCE_DIR}/sql/queries.sql 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/sql
    COMMENT "Generating models with sqlc and formatting"
)
add_custom_target(sqlc DEPENDS ${CMAKE_SOURCE_DIR}/src/models.h)

add_executable(${PROJECT_NAME}
  src/main.c
  src/queries.c
  vendor/sqlite3.c
  vendor/unit.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/vendor
)

# Ensure schema.h is generated before building the executable
add_dependencies(${PROJECT_NAME}
    generate_schema
    sqlc
)

target_link_libraries(${PROJECT_NAME} PRIVATE ecewo)

