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
std::string textBuffer = "Welcome to coditor";
GLFWwindow* window;

void Run()
{
    // Menu bar
    float menuBarHeight = 0.0f;
    if (ImGui::BeginMainMenuBar()) {
        menuBarHeight = ImGui::GetWindowSize().y;
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) { /* load file */ }
            if (ImGui::MenuItem("Save")) { /* save file */ }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Get full window size and set editor window position/size
    ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(display_w, display_h - menuBarHeight));

    ImGuiWindowFlags editor_flags = ImGuiWindowFlags_NoResize |
                                     ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Filename", nullptr, editor_flags);

    // Make InputTextMultiline fill the content region of the window
    ImVec2 contentSize = ImGui::GetContentRegionAvail();
    ImGui::InputTextMultiline("##editor", &textBuffer[0], textBuffer.size() + 1024, contentSize);

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
