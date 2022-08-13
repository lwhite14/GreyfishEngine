#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "UserInterface.h"

class GameWindow : public UserInterface
{
private:
    unsigned int m_gameViewFbo;
    int m_width, m_height;
    bool m_hasResized;

public:
    GameWindow(int width, int height) : m_width{ width }, m_height{ height }, m_gameViewFbo{ 0 }, m_hasResized{ false }
    {
        
    }

    void DrawElements() 
    {
        ImGui::BeginChild("Game View", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
        ImGui::Image(reinterpret_cast<ImTextureID>(m_gameViewFbo), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }

    void SetGameViewFbo(unsigned int gameViewFbo) 
    {
        m_gameViewFbo = gameViewFbo;
    }

    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }
};

#endif //GAMEWINDOW_H