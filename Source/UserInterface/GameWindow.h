#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
    ImVec2 m_offset;
    GLFWwindow* m_window;

public:
    GameWindow(GLFWwindow* window, int width, int height) : m_window{ window }, m_width{ width }, m_height{ height }, m_gameViewFbo{ 0 }, m_hasResized{ false }
    {
        m_offset = ImVec2{0, 0};
    }

    void DrawElements() 
    {
        ImGui::BeginChild("Game View", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
        ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_gameViewFbo), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), ImVec2(0, 1), ImVec2(1, 0), 0);
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            m_offset.x += ImGui::GetIO().MouseDelta.x;
            m_offset.y += ImGui::GetIO().MouseDelta.y;
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        ImGui::EndChild();
    }

    void SetGameViewFbo(unsigned int gameViewFbo) 
    {
        m_gameViewFbo = gameViewFbo;
    }

    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }
    glm::vec2 GetOffset() { return glm::vec2(m_offset.x, m_offset.y); }
};

#endif //GAMEWINDOW_H