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
        bool p_open = true;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 window_pos, window_pos_pivot;
        window_pos_pivot.x = (0 & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (0 & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
        
        if (ImGui::Begin("Example: Simple overlay", &p_open, window_flags))
        {
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        }
        ImGui::End();
    }

    Framerate()
    {
    
    }
};

#endif //FRAMERATE_H