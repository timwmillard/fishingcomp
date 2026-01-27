cmake_minimum_required(VERSION 3.15)

project(fishingcomp C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build bin2c tool for schema generation
add_executable(bin2c tool/bin2c.c)
set_target_properties(bin2c PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tool
)
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/src/db/schema.h
    COMMAND $<TARGET_FILE:bin2c> ${CMAKE_SOURCE_DIR}/sql/schema.sql ${CMAKE_SOURCE_DIR}/src/db/schema.h sql_schema_sql
    DEPENDS ${CMAKE_SOURCE_DIR}/sql/schema.sql bin2c
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating db/schema.h from sql/schema.sql using bin2c"
)
add_custom_target(generate_schema DEPENDS ${CMAKE_SOURCE_DIR}/src/db/schema.h)

# Build sql2c tool
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/tool/sql2c
    COMMAND go build -o ${CMAKE_BINARY_DIR}/tool/sql2c .
    DEPENDS ${CMAKE_SOURCE_DIR}/tool/sql2c/main.go
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tool/sql2c
    COMMENT "Building sql2c tool"
)
add_custom_target(build_sql2c DEPENDS ${CMAKE_BINARY_DIR}/tool/sql2c)

# Generate SQL queries with sql2c
add_custom_command(
    OUTPUT
        ${CMAKE_SOURCE_DIR}/src/db/queries.h
        ${CMAKE_SOURCE_DIR}/src/db/queries.c
        ${CMAKE_SOURCE_DIR}/src/db/models.h
    COMMAND ${CMAKE_BINARY_DIR}/tool/sql2c -config config.yaml
    DEPENDS
        ${CMAKE_BINARY_DIR}/tool/sql2c
        ${CMAKE_SOURCE_DIR}/sql/schema.sql
        ${CMAKE_SOURCE_DIR}/sql/queries.sql
        ${CMAKE_SOURCE_DIR}/sql/config.yaml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/sql
    COMMENT "Generating db/queries.h, db/queries.c, db/models.h with sql2c"
)
add_custom_target(generate_queries DEPENDS
    ${CMAKE_SOURCE_DIR}/src/db/queries.h
    ${CMAKE_SOURCE_DIR}/src/db/queries.c
    ${CMAKE_SOURCE_DIR}/src/db/models.h
)

####### BUILD APP ###############
include_directories(.)
include_directories(deps)
include_directories(deps/cimgui)

# Platform-specific configuration
if(APPLE)
    set(BACKEND -DSOKOL_METAL)
    set(PLATFORM_LIBS "-framework Cocoa" "-framework QuartzCore" "-framework Metal" "-framework MetalKit")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ObjC")
elseif(WIN32)
    set(BACKEND -DSOKOL_D3D11)
    set(PLATFORM_LIBS gdi32 ole32 d3d11 dxgi)
    add_definitions(-D_WIN32_WINNT=0x0601)
else()
    # Linux
    set(BACKEND -DSOKOL_GLCORE)
    set(PLATFORM_LIBS GL m pthread dl)
    add_definitions(-D_GNU_SOURCE)
endif()

add_definitions(${BACKEND})

add_executable(${PROJECT_NAME}
    src/main.c
    src/ui_window.c
    deps/sqlite3.c
    deps/deps.c
    deps/deps.cpp
    deps/cimgui/cimgui.cpp
    deps/cimgui/imgui/imgui.cpp
    deps/cimgui/imgui/imgui_draw.cpp
    deps/cimgui/imgui/imgui_tables.cpp
    deps/cimgui/imgui/imgui_widgets.cpp
    deps/cimgui/imgui/imgui_demo.cpp
)

# Ensure schema.h is generated before building the executable
add_dependencies(${PROJECT_NAME}
    generate_schema
    generate_queries
)

target_link_libraries(${PROJECT_NAME} ${PLATFORM_LIBS})

