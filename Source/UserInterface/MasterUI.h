#ifndef MASTERUI_H
#define MASTERUI_H

#include <GLFW/glfw3.h>
#include <vector>
#include "UserInterface.h"

class MasterUI 
{
private:
    std::vector<UserInterface*> m_windows;

public:
    void Init(GLFWwindow* window);
    void PerFrame();
    void CleanUp();
};

#endif //MASTERUI_H