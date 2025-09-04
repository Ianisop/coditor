#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
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
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) { /* load file */ }
            if (ImGui::MenuItem("Save")) { /* save file */ }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Editor
    ImGui::Begin("asfasgagerage");
    ImGui::InputTextMultiline("##editor", &textBuffer[0], textBuffer.size() + 1024, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 20));
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
