// UI Window

#include "sokol_app.h"
#include "sqlite3.h"

#define ARENA_IMPLEMENTATION
#include "arena.h"

#include "db/queries.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"

extern sqlite3 *db;

static struct {
    // GUI
    bool show_competitors;
    bool show_boats;

    bool show_demo;
    bool dock_setup_done;

    Competitor competitor;
    Arena arena;

} window_state = {0};

static sql_text arena_copy_text(Arena *a, sql_text src) {
    return (sql_text){
        .data = arena_memdup(a, src.data, src.len),
        .len = src.len,
    };
}

void on_get_competitor(Competitor* comp, void* ctx) {
    arena_reset(&window_state.arena);

    window_state.competitor.id = comp->id;
    window_state.competitor.competitor_no = arena_copy_text(&window_state.arena, comp->competitor_no);
    window_state.competitor.first_name = arena_copy_text(&window_state.arena, comp->first_name);
    window_state.competitor.last_name = arena_copy_text(&window_state.arena, comp->last_name);
    window_state.competitor.email = arena_copy_text(&window_state.arena, comp->email);
    window_state.competitor.mobile = arena_copy_text(&window_state.arena, comp->mobile);
    window_state.competitor.phone = arena_copy_text(&window_state.arena, comp->phone);
    window_state.competitor.address1 = arena_copy_text(&window_state.arena, comp->address1);
    window_state.competitor.address2 = arena_copy_text(&window_state.arena, comp->address2);
    window_state.competitor.suburb = arena_copy_text(&window_state.arena, comp->suburb);
    window_state.competitor.state = arena_copy_text(&window_state.arena, comp->state);
    window_state.competitor.postcode = arena_copy_text(&window_state.arena, comp->postcode);
    window_state.competitor.boat_id = comp->boat_id;
}

void refresh_data() {
    int rc = get_competitor(db, 1, on_get_competitor, NULL);
}


void ui_reset_layout(ImGuiID dockspace_id)
{
    window_state.show_competitors = true;
    window_state.show_boats = false;

    igDockBuilderRemoveNode(dockspace_id);
    igDockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    igDockBuilderSetNodeSize(dockspace_id, igGetMainViewport()->Size);

    igDockBuilderDockWindow("Competitors Details", dockspace_id);
    // igDockBuilderDockWindow("Chart of Accounts", dockspace_id);
    // igDockBuilderDockWindow("General Ledger", dockspace_id);
    // igDockBuilderDockWindow("Dear ImGui Demo", dockspace_id);
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
            igText("Please enter your business name:");
            igText("Competitor: %s", window_state.competitor.first_name);
            // igInputText("##business_name", state.business.name, sizeof(state.business.name), ImGuiInputTextFlags_None, NULL, NULL);

            igSeparator();

            if (igButton("Save", (ImVec2){80, 0})) {
                // if (state.db)
                //     db_save_business(state.db, &state.business);
            }
            igSameLine(0, -1);
            if (igButton("Reset", (ImVec2){80, 0})) {
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

