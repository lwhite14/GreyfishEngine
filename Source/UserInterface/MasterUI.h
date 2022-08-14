#ifndef MASTERUI_H
#define MASTERUI_H

#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

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
    
public:
    MasterUI();
    MasterUI(GLFWwindow* window, ImVec2 size);
    void Init();
    void PerFrame();
    void CleanUp();

    GLFWwindow* GetWindow();
    ImVec2 GetSize();
    ImVec2 GetGameViewSize();
    ImVec2 GetOptionsViewSize();
    ImVec2 GetOffset();
    unsigned int GetGameViewFBO();

    void SetWindow(GLFWwindow* window);
    void SetSize(ImVec2 size);
    void SetGameViewSize(ImVec2 gameViewSize);
    void SetOptionsViewSize(ImVec2 optionsViewSize);
    void SetOffset(ImVec2 offset);
    void SetGameViewFBO(unsigned int gameViewFbo);
};

#endif //MASTERUI_H