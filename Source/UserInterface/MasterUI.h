#ifndef MASTERUI_H
#define MASTERUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/imgui_impl_glfw.h"
#include "../Dependencies/imgui/imgui_impl_opengl3.h"
#include "../Motion.h"
#include "../SceneObjects/Component.h"

class MasterUI 
{
private:
    GLFWwindow* m_window;
    ImGuiWindowFlags m_windowFlags;
    bool m_hasResized;
    ImVec2 m_size;
    ImVec2 m_gameViewSize;
    ImVec2 m_optionsViewSize;
    ImVec2 m_offset;
    unsigned int m_gameViewFbo;
    float m_mouseWheel;
    Motion m_camMotion;
    
public:
    MasterUI();
    MasterUI(GLFWwindow* window, ImVec2 size);
    void Init();
    void PerFrame(std::vector<Component*> components);
    void CleanUp();

    GLFWwindow* GetWindow();
    ImVec2 GetSize();
    ImVec2 GetGameViewSize();
    ImVec2 GetOptionsViewSize();
    ImVec2 GetOffset();
    unsigned int GetGameViewFBO();
    float GetMouseWheel();
    Motion GetCamMotion();

    void SetWindow(GLFWwindow* window);
    void SetSize(ImVec2 size);
    void SetGameViewSize(ImVec2 gameViewSize);
    void SetOptionsViewSize(ImVec2 optionsViewSize);
    void SetOffset(ImVec2 offset);
    void SetGameViewFBO(unsigned int gameViewFbo);
    void SetMouseWheel(float mouseWheel);
    void SetCamMotion(Motion motion);
};

#endif //MASTERUI_H