#include <stdio.h>
#include <stdlib.h>

#include "sqlite3.h"
#include "db/schema.h"
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "sokol_imgui.h"

void ui_window(void);
void refresh_data();

// Clipboard callbacks for ImGui
static const char* imgui_get_clipboard(ImGuiContext* ctx) {
    (void)ctx;
    return sapp_get_clipboard_string();
}

static void imgui_set_clipboard(ImGuiContext* ctx, const char* text) {
    (void)ctx;
    sapp_set_clipboard_string(text);
}

static struct {
    sg_pass_action pass_action;
} state = {0};

sqlite3 *db;

void ui_draw()
{
    ui_window();
}

void frame(void)
{
    simgui_new_frame(&(simgui_frame_desc_t){
        .width = sapp_width(),
        .height = sapp_height(),
        .delta_time = sapp_frame_duration(),
        .dpi_scale = sapp_dpi_scale(),
    });

    ui_draw();

    sg_begin_pass(&(sg_pass){
            .action = state.pass_action,
            .swapchain = sglue_swapchain(),
        });

    simgui_render();

    sg_end_pass();
    sg_commit();
}

void event(const sapp_event* ev)
{
    // Development only
    if (ev->type == SAPP_EVENTTYPE_KEY_DOWN && ev->key_code == SAPP_KEYCODE_ESCAPE) {
        sapp_quit();
    }

    simgui_handle_event(ev);
}

void cleanup(void)
{
    simgui_shutdown();
    sg_shutdown();
}

void init(void)
{
    sg_setup(&(sg_desc){
            .environment = sglue_environment(),
            .logger.func = slog_func,
            });

    simgui_setup(&(simgui_desc_t){.no_default_font = true});

    // Enable docking
    ImGuiIO* io = igGetIO_Nil();
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Set up clipboard callbacks
    ImGuiPlatformIO* pio = igGetPlatformIO_Nil();
    pio->Platform_GetClipboardTextFn = imgui_get_clipboard;
    pio->Platform_SetClipboardTextFn = imgui_set_clipboard;

    // Use cimgui constructor to get properly initialized config
    ImFontConfig* font_cfg = ImFontConfig_ImFontConfig();

    // Add font - sokol_imgui 1.92+ handles atlas texture automatically
    // ImFontAtlas_AddFontDefault(io->Fonts, font_cfg);
    // Or for custom font:
    ImFontAtlas_AddFontFromFileTTF(io->Fonts, "extra_font/Roboto-Regular.ttf", 20.0f, font_cfg, NULL);

    ImFontConfig_destroy(font_cfg);


    state.pass_action = (sg_pass_action) {
        .colors[0] = {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = {0.2, 0.2, 0.2, 1.0},
        },
    };
}

void db_open(const char *db_name) {
    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK) {
        printf("Database open failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    // Execute embedded schema
    char *err_msg = NULL;
    rc = sqlite3_exec(db, (char*)sql_schema_sql_data, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Database schema execution failed: %s", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        exit(1);
    }
}

sapp_desc sokol_main(int argc, char *argv[])
{
    const char *db_name = "fishingcomp.db";
    if (argc > 1) db_name = argv[1]; 
    db_open(db_name);
    refresh_data();

    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 800,
        .height = 600,
        .window_title = "Fishing Comp",
        .enable_clipboard = true,
        .clipboard_size = 8192,
    };
}
