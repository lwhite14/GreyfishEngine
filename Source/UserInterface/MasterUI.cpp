#include "MasterUI.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "Framerate.h"
#include "GameWindow.h"

void MasterUI::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    m_framerate = new Framerate();
    m_gameWindow = new GameWindow(900, 600);
}

void MasterUI::PerFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();
    m_framerate->DrawElements();
    m_gameWindow->DrawElements();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void MasterUI::CleanUp() 
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void MasterUI::SetGameWindowRender(unsigned int gameViewFbo)
{
    m_gameWindow->SetGameViewFbo(gameViewFbo);
}

Framerate* MasterUI::GetFramerate() { return m_framerate; }
GameWindow* MasterUI::GetGameWindow() { return m_gameWindow; }
