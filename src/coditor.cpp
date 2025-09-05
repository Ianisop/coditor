#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <cfloat>
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

float toolbar_height     = 30.0f;
float status_bar_height  = 30.0f;
float file_explorer_width = 200.0f;

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

    // Example buttons
    ImGui::LabelText("Coditor","");
    ImGui::SameLine();
    if (ImGui::Button("-")) {}
    ImGui::SameLine();
    if (ImGui::Button("O")) {}
    ImGui::SameLine();
    if (ImGui::Button("x"))
    {
        glfwSetWindowShouldClose(window, true);
    }



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
    ImGui::Text("Current Directory");
    ImGui::Separator();
    for(auto & file : files) {
        if (file.is_directory) {
            ImGui::Text("[DIR] %s", file.name.c_str());
        } else {
            if (ImGui::Selectable(file.name.c_str())) {
                text_buffer = file.contents;
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
    ImGui::InputTextMultiline("##editor", &text_buffer[0], text_buffer.size() + 1024, content_size);
    ImGui::End();

    // --- Status bar ---
    ImGui::SetNextWindowPos(ImVec2(0, display_h - status_bar_height));
    ImGui::SetNextWindowSize(ImVec2(display_w, status_bar_height));
    ImGuiWindowFlags status_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("StatusBar", nullptr, status_flags);
    ImGui::Text("Ln 1, Col 1    |    UTF-8    |    Ready");
    ImGui::End();
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

    files = FileExplorer::ListFiles(".");

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
