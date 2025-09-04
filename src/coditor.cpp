#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <cfloat>
#include <fstream>
#include <string>
#include "coditor.h"


int display_w = 1280;
int display_h = 720;
std::string text_buffer = "Welcome to coditor";
GLFWwindow* window;

void Run()
{
    // Menu bar
    float menu_bar_height = 0.0f;
    if (ImGui::BeginMainMenuBar()) {
        menu_bar_height = ImGui::GetWindowSize().y;
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) { /* load file */ }
            if (ImGui::MenuItem("Save")) { /* save file */ }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Sizes
    float file_explorer_width = 250.0f;  // left panel width
    float status_bar_height   = 20.0f;   // bottom bar height
    float editor_width       = display_w - file_explorer_width;
    float editor_height      = display_h - menu_bar_height - status_bar_height;

    // File Tree
    ImGui::SetNextWindowPos(ImVec2(0, menu_bar_height));
    ImGui::SetNextWindowSize(ImVec2(file_explorer_width, editor_height));
    ImGuiWindowFlags side_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::Begin("Files", nullptr, side_flags);
    ImGui::Text("File Explorer");
    ImGui::Separator();

    //TODO: Add functionality to show and find files 
    // Example mock files
    if (ImGui::Selectable("main.cpp")) { /* load file */ }
    if (ImGui::Selectable("coditor.h")) { /* load file */ }
    if (ImGui::Selectable("CMakeLists.txt")) { /* load file */ }
    ImGui::End();

    // Editor
    ImGui::SetNextWindowPos(ImVec2(file_explorer_width, menu_bar_height));
    ImGui::SetNextWindowSize(ImVec2(editor_width, editor_height));
    ImGuiWindowFlags editor_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Editor", nullptr, editor_flags);
    ImVec2 content_size = ImGui::GetContentRegionAvail();
    ImGui::InputTextMultiline("##editor", &text_buffer[0], text_buffer.size() + 1024, content_size);
    ImGui::End();

    // Status bar
    ImGui::SetNextWindowPos(ImVec2(0, display_h - status_bar_height));
    ImGui::SetNextWindowSize(ImVec2(display_w, status_bar_height));
    ImGuiWindowFlags status_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("StatusBar", nullptr, status_flags);

    //TODO: make the status bar actually show metadata
    ImGui::Text("Ln 1, Col 1    |    UTF-8    |    Ready");
    ImGui::End();
}





int main() 
{
    // Init GLFW + OpenGL
    glfwInit();
    window = glfwCreateWindow(display_w, display_h, "coditor", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();


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
