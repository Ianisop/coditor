#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include "coditor.h"

int main() {
    // Init GLFW + OpenGL
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "coditor", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    std::string textBuffer = "Welcome to coditor";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Menu bar
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open")) { /* load file */ }
                if (ImGui::MenuItem("Save")) { /* save file */ }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // Editor
        ImGui::Begin("Editor");
        ImGui::InputTextMultiline("##editor", &textBuffer[0], textBuffer.size() + 1024,
            ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 20));
        ImGui::End();

        // Render
        ImGui::Render();
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
