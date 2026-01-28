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
    if (!db) return;
    data.competitor_id = 1;
    int rc = get_competitor(db, data.competitor_id, on_get_competitor, NULL);
}

void save_data() {
    if (!db) return;
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

    igDockBuilderDockWindow("Competitors", dockspace_id);
    // igDockBuilderDockWindow("Boats", dockspace_id);
    // igDockBuilderDockWindow("Catches", dockspace_id);
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

    // Competitors Window
    if (window_state.show_competitors) {
        static bool edit_mode = false;
        if (igBegin("Competitors", &window_state.show_competitors, ImGuiWindowFlags_None)) {

            static int selected = 0;
            // Left panel
            {
                igBeginChild_Str("left pane", (ImVec2){150, 0}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX, 0);
                    for (int i = 0; i < 100; i++)
                    {
                        char label[128];
                        sprintf(label, "MyObject %d", i);
                        if (igSelectable_Bool(label, selected == i, ImGuiSelectableFlags_SelectOnNav, (ImVec2){0, 0}))
                            selected = i;
                    }
                igEndChild();
            }

            igSameLine(0, 0);

            // Right panel
            {
                igBeginGroup();
                igBeginChild_Str("item view", (ImVec2){0, -igGetFrameHeightWithSpacing()}, 0, 0); // Leave room for 1 line below us
                if (igButton("Edit", (ImVec2){80, 0})) {
                    edit_mode = !edit_mode;
                }

                int flags = ImGuiInputTextFlags_None;
                if (!edit_mode) flags |= ImGuiInputTextFlags_ReadOnly;

                igText("Competitor No. %d", data.competitor.id);
                igText("First name");
                igInputText("##first_name", data.competitor.first_name, MAX_STR_LEN, flags, NULL, NULL);
                igText("Last name");
                igInputText("##last_name", data.competitor.last_name, MAX_STR_LEN, flags, NULL, NULL);

                igSeparator();
                igEndChild();
                
                if (edit_mode) {
                    if (igButton("Save", (ImVec2){80, 0})) {
                        save_data();
                        edit_mode = false;
                    }
                    igSameLine(0, -1);
                    if (igButton("Reset", (ImVec2){80, 0})) {
                        refresh_data();
                    }
                }
                igSameLine(0, 0);
                igEndGroup();
            }
        }
        igEnd();
    }

    // Demo Window
    if (window_state.show_demo) {
        igShowDemoWindow(&window_state.show_demo);
    }
}

// // Left
// static int selected = 0;
// {
//     ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
//     for (int i = 0; i < 100; i++)
//     {
//         char label[128];
//         sprintf(label, "MyObject %d", i);
//         if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SelectOnNav))
//             selected = i;
//     }
//     ImGui::EndChild();
// }
// ImGui::SameLine();
//
// // Right
// {
//     ImGui::BeginGroup();
//     ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
//     ImGui::Text("MyObject: %d", selected);
//     ImGui::Separator();
//     if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
//     {
//         if (ImGui::BeginTabItem("Description"))
//         {
//             ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
//             ImGui::EndTabItem();
//         }
//         if (ImGui::BeginTabItem("Details"))
//         {
//             ImGui::Text("ID: 0123456789");
//             ImGui::EndTabItem();
//         }
//         ImGui::EndTabBar();
//     }
//     ImGui::EndChild();
//     if (ImGui::Button("Revert")) {}
//     ImGui::SameLine();
//     if (ImGui::Button("Save")) {}
//     ImGui::EndGroup();
// }
