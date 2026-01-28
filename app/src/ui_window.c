// UI Window

#include <string.h>
#include "sokol_app.h"
#include "sqlite3.h"

#define ARENA_IMPLEMENTATION
#include "arena.h"

#include "db/queries.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"

extern sqlite3 *db;

#define MAX_STR_LEN 256

static struct {
    struct {
        int id;
        char first_name[MAX_STR_LEN];
        char last_name[MAX_STR_LEN];
    } competitor;
    int competitor_id;
} data = {0};

static struct {
    // GUI
    bool show_competitors;
    bool competitor_edit;

    bool show_boats;

    bool show_demo;
    bool dock_setup_done;

} window_state = {0};

static void copy_sql_text(char *dst, size_t dst_size, sql_text src) {
    size_t len = src.len < dst_size - 1 ? src.len : dst_size - 1;
    memcpy(dst, src.data, len);
    dst[len] = '\0';
}

void on_get_competitor(Competitor* comp, void* ctx) {
    data.competitor.id = comp->id;
    copy_sql_text(data.competitor.first_name, MAX_STR_LEN, comp->first_name);
    copy_sql_text(data.competitor.last_name, MAX_STR_LEN, comp->last_name);
}

void refresh_data() {
    data.competitor_id = 1;
    int rc = get_competitor(db, data.competitor_id, on_get_competitor, NULL);
}
void save_data() {
    UpdateCompetitorParams params = {
        .first_name.data = (sql_byte*)data.competitor.first_name,
        .first_name.len = strlen(data.competitor.first_name), 

        .last_name.data = (sql_byte*)data.competitor.last_name,
        .last_name.len = strlen(data.competitor.last_name), 
        .id = data.competitor_id,
    };
    int rc = update_competitor(db, &params, NULL, NULL);
    refresh_data();
}


void ui_reset_layout(ImGuiID dockspace_id)
{
    window_state.show_competitors = true;
    window_state.show_boats = false;

    igDockBuilderRemoveNode(dockspace_id);
    igDockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    igDockBuilderSetNodeSize(dockspace_id, igGetMainViewport()->Size);

    igDockBuilderDockWindow("Competitors Details", dockspace_id);
    // igDockBuilderDockWindow("Boat Details", dockspace_id);
    // igDockBuilderDockWindow("Catch Details", dockspace_id);
    igDockBuilderFinish(dockspace_id);
}

void ui_window(void)
{
    ImGuiID dockspace_id = igGetID_Str("MainDockSpace");

    // Main Menu
    if (igBeginMainMenuBar()) {
        if (igBeginMenu("File", true)) {
            if (igMenuItem_Bool("New", "Ctrl+N", false, true)) {
                // Handle new file
            }
            if (igMenuItem_Bool("Open", "Ctrl+O", false, true)) {
                // Handle open file
            }
            if (igMenuItem_Bool("Save", "Ctrl+S", false, true)) {
                // Handle save file
            }
            igSeparator();
            if (igMenuItem_Bool("Import", "", false, true)) {
            }
            if (igMenuItem_Bool("Export", "", false, true)) {
            }
            igSeparator();
            if (igMenuItem_Bool("Quit", "Ctrl+Q", false, true)) {
                sapp_quit();
            }
            igEndMenu();
        }
        if (igBeginMenu("Fishing", true)) {
            if (igMenuItem_Bool("Competitors", "", false, true)) {
                window_state.show_competitors = true;
            }
            if (igMenuItem_Bool("Boats", "", false, true)) {
                window_state.show_boats = true;
                igSetWindowFocus_Str("Chart of Accounts");
            }
            igEndMenu();
        }
        if (igBeginMenu("Reports", true)) {
            if (igMenuItem_Bool("Murray Cod Challenge", "", false, true)) {
            }
            if (igMenuItem_Bool("Yellowbelly Challenge", "", false, true)) {
            }
            if (igMenuItem_Bool("Longest Murray Cod", "", false, true)) {
            }
            if (igMenuItem_Bool("Longest Yellowbelly", "", false, true)) {
            }
            igEndMenu();
        }
        if (igBeginMenu("View", true)) {
            if (igMenuItem_Bool("Refresh Data", "", false, true)) {
                refresh_data();
            }
            igSeparator();
            if (igMenuItem_Bool("Reset Layout", "", false, true)) {
                ui_reset_layout(dockspace_id);
            }
            igSeparator();
            if (igMenuItem_Bool("Zoom In", "", false, true)) {
            }
            if (igMenuItem_Bool("Zoom Out", "", false, true)) {
            }
            if (igMenuItem_Bool("Reset Zoom", "", false, true)) {
            }
            igEndMenu();
        }
        if (igBeginMenu("Debug", true)) {
            if (igMenuItem_Bool("Demo", "", false, true)) {
                window_state.show_demo = true;
            }
            igEndMenu();
        }
        igEndMainMenuBar();
    }

    // Dockspace
    igDockSpaceOverViewport(dockspace_id, igGetMainViewport(), ImGuiDockNodeFlags_None, NULL);

    // Setup docking layout on first frame
    if (!window_state.dock_setup_done) {
        ui_reset_layout(dockspace_id);

        window_state.dock_setup_done = true;
    }

    // Business Details Window
    if (window_state.show_competitors) {
        if (igBegin("Competitors Details", &window_state.show_competitors, ImGuiWindowFlags_None)) {
            if (igButton("Edit", (ImVec2){80, 0})) {
                window_state.competitor_edit = true;
            }

            int flags = ImGuiInputTextFlags_None;
            if (!window_state.competitor_edit) flags |= ImGuiInputTextFlags_ReadOnly;

            igText("Competitor No. %d", data.competitor.id);
            igText("First name");
            igInputText("##first_name", data.competitor.first_name, MAX_STR_LEN, flags, NULL, NULL);
            igText("Last name");
            igInputText("##last_name", data.competitor.last_name, MAX_STR_LEN, flags, NULL, NULL);

            igSeparator();

            if (igButton("Save", (ImVec2){80, 0})) {
                save_data();
                // if (state.db)
                //     db_save_business(state.db, &state.business);
            }
            igSameLine(0, -1);
            if (igButton("Reset", (ImVec2){80, 0})) {
                refresh_data();
                // if (state.db)
                //     db_get_business(state.db, &state.business);
                // // state.show_business = false;
            }
        }
        igEnd();
    }

    // Demo Window
    if (window_state.show_demo) {
        igShowDemoWindow(&window_state.show_demo);
    }
}

