// UI Window

#include "sokol_app.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"

/*** Global State ***/
static struct {
    // GUI
    bool show_competitors;
    bool show_boats;

    bool show_demo;
    bool dock_setup_done;


} window_state = {0};

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

