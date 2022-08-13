#ifndef MASTERUI_H
#define MASTERUI_H

#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Framerate.h"
#include "GameWindow.h"

class MasterUI 
{
private:
    Framerate* m_framerate;
    GameWindow* m_gameWindow;
    ImGuiWindowFlags m_windowFlags;
    bool m_hasResized;
    int m_width, m_height;

public:
    MasterUI();
    MasterUI(int width, int height);
    void Init(GLFWwindow* window);
    void PerFrame();
    void CleanUp();

    void SetGameWindowRender(unsigned int gameViewFbo);
    Framerate* GetFramerate();
    GameWindow* GetGameWindow();
    glm::vec2 GetGameWindowOffset();
};

#endif //MASTERUI_H