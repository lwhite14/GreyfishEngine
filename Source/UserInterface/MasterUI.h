#ifndef MASTERUI_H
#define MASTERUI_H

#include <GLFW/glfw3.h>
#include <vector>
#include "Framerate.h"
#include "GameWindow.h"

class MasterUI 
{
private:
    Framerate* m_framerate;
    GameWindow* m_gameWindow;

public:
    void Init(GLFWwindow* window);
    void PerFrame();
    void CleanUp();

    void SetGameWindowRender(unsigned int gameViewFbo);
    Framerate* GetFramerate();
    GameWindow* GetGameWindow();
};

#endif //MASTERUI_H