#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <cfloat>
#include <cstdio>
#include <fstream>
#include <string>
#include "coditor.h"
#include "file_explorer.h"


int display_w = 1280;
int display_h = 720;
std::string text_buffer = "";
GLFWwindow* window;

bool dragging = false;
ImVec2 drag_offset;
double offsetX, offsetY;

std::vector<FileExplorer::FileInfo> files;
FileExplorer::Directory current_directory;
FileExplorer::FileInfo current_file; // Currently opened file

float toolbar_height     = 30.0f;
float status_bar_height  = 30.0f;
float file_explorer_width = 200.0f;
int line_count = 1;

//TOOD: add a tab bar so you can tab in between multiple files, add more buffers too
void DrawToolbar()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(display_w, toolbar_height));

    ImGuiWindowFlags flags =
                             ImGuiWindowFlags_NoResize   |
                             ImGuiWindowFlags_NoMove     |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoScrollbar|
                             ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("Coditor", nullptr, flags);

    float windowWidth = ImGui::GetWindowSize().x;
    const char* title = "Coditor";
    ImVec2 textSize = ImGui::CalcTextSize(title);

    // Width of all buttons
    float buttonWidth = 30.0f;
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    float totalButtonsWidth = buttonWidth * 3 + spacing * 2;

    // Start a horizontal layout
    ImGui::BeginGroup();

    // Left spacer
    ImGui::Dummy(ImVec2((windowWidth - textSize.x - totalButtonsWidth) * 0.5f, 0.0f));
    ImGui::SameLine();

    // Title
    ImGui::TextUnformatted(title);
    ImGui::SameLine();

    // Right spacer to push buttons to the right
    ImGui::Dummy(ImVec2((windowWidth - textSize.x - totalButtonsWidth) * 0.5f, 0.0f));
    ImGui::SameLine();

    // Buttons
    if (ImGui::Button("-")) {}
    ImGui::SameLine();
    if (ImGui::Button("O")) {}
    ImGui::SameLine();
    if (ImGui::Button("x"))
    {
        glfwSetWindowShouldClose(window, true);
    }

    ImGui::EndGroup();





    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        dragging = true;

        double mx, my;
        glfwGetCursorPos(window, &mx, &my); // mouse inside window
        offsetX = mx;
        offsetY = my;
    }

    if (dragging && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);

        // Get mouse in screen coords
        int wx, wy;
        glfwGetWindowPos(window, &wx, &wy);
        int screenX = wx + (int)mx;
        int screenY = wy + (int)my;

        // Move window so the cursor stays at the same spot inside it
        glfwSetWindowPos(window, screenX - (int)offsetX, screenY - (int)offsetY);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        dragging = false;
    }



    ImGui::End();
}

bool SaveChangesToFile(const std::string& filename, const std::string& contents) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        return false; // Failed to open file
    }
    file << contents;
    return true; // Successfully saved
}

static int MyCallbackResize(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        std::string* str = (std::string*)data->UserData;
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    return 0;
}

static int TextEditCallback(ImGuiInputTextCallbackData* data)
{
    // You can store cursor position for later
    int cursor = data->CursorPos;

    // Count lines and column from buffer start → cursor
    int line = 1;
    int col  = 1;
    for (int i = 0; i < cursor; i++) {
        if (data->Buf[i] == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
    }

    // Store it in user data (e.g., a struct or global)
    auto* pos = (std::pair<int,int>*)data->UserData;
    *pos = { line, col };

    return 0;
}


void Run()
{

    float editor_width  = display_w - file_explorer_width;
    float editor_height = display_h - toolbar_height - status_bar_height;

    DrawToolbar();

    // --- File Explorer ---
    ImGui::SetNextWindowPos(ImVec2(0, toolbar_height));
    ImGui::SetNextWindowSize(ImVec2(file_explorer_width, editor_height));
    ImGuiWindowFlags side_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::Begin("Files", nullptr, side_flags);
    ImGui::Text("%s", current_directory.name.c_str());
    ImGui::Separator();
    for(auto & file : files) {
        //TODO: make this a display all the files in the dir as selectables
        if (file.is_directory) {
            ImGui::Text("[DIR] %s", file.name.c_str());
        } else {
            if (ImGui::Selectable(file.name.c_str())) {
                text_buffer = file.contents;
                current_file = file;
            }
        }
    }
    ImGui::End();

    // --- Editor ---
    ImGui::SetNextWindowPos(ImVec2(file_explorer_width, toolbar_height));
    ImGui::SetNextWindowSize(ImVec2(editor_width, editor_height));
    ImGuiWindowFlags editor_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Editor", nullptr, editor_flags);
    ImVec2 content_size = ImGui::GetContentRegionAvail();
    ImGui::InputTextMultiline("##editor",
    (char*)text_buffer.c_str(), text_buffer.capacity() + 1,
    content_size,
    ImGuiInputTextFlags_CallbackResize,
    MyCallbackResize, &text_buffer);

    ImGui::End();

    // --- Status bar ---
    ImGui::SetNextWindowPos(ImVec2(0, display_h - status_bar_height));
    ImGui::SetNextWindowSize(ImVec2(display_w, status_bar_height));
    ImGuiWindowFlags status_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("StatusBar", nullptr, status_flags);
    ImGui::Text("%d  |    UTF-8    |    Ready", line_count);
    ImGui::End();



    if(ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_S)) {
        printf("Ctrl+S pressed\n");
        // Save the file{
        if (SaveChangesToFile(current_directory.name + "/" + current_file.name, text_buffer)) {
            // Successfully saved
             printf("File saved: %s\n", current_file.name.c_str());
        } else {
                // Handle save error
                printf("Failed to save file: %s\n", current_file.name.c_str());
            }

    }
}
    




int main() 
{
    // Init GLFW + OpenGL
    glfwInit();
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    window = glfwCreateWindow(display_w, display_h, "coditor", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    current_directory = FileExplorer::GetDirectoryInfo("./testDir");
    files = FileExplorer::ListFiles(current_directory.name);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Run();

        // Render
        ImGui::Render();

        // Clear framebuffer
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}
