#ifndef FRAMERATE_H
#define FRAMERATE_H

#include <GLFW/glfw3.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "UserInterface.h"

class Framerate : public UserInterface
{
public:
	void DrawElements() 
    {
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        windowFlags |= ImGuiWindowFlags_NoBackground;
        windowFlags |= ImGuiWindowFlags_NoResize;

        ImGui::Begin("Framerate", NULL, windowFlags);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2(100, 100));
        ImGui::End();
    }

    Framerate() 
    {
    
    }
};

#endif //FRAMERATE_H