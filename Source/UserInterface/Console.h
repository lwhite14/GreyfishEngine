#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>
#include <string>
#include "../Dependencies/imgui/imgui.h"

class Console 
{
private:
	inline static std::vector<std::string> messages;

public:
    inline static bool isOn = false;

	inline static void PerFrame() 
    {
        static bool isFirstFrame = true;

        ImGuiWindowFlags windowFlags = 0;
        //windowFlags |= ImGuiWindowFlags_NoResize;
        //windowFlags |= ImGuiWindowFlags_NoTitleBar;
        //windowFlags |= ImGuiWindowFlags_NoScrollbar;
        //windowFlags |= ImGuiWindowFlags_MenuBar;
        //windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoCollapse;
        //windowFlags |= ImGuiWindowFlags_NoNav;
        //windowFlags |= ImGuiWindowFlags_NoBackground;
        //windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        //windowFlags |= ImGuiWindowFlags_NoDocking;
        //windowFlags |= ImGuiWindowFlags_UnsavedDocument;

        if (isFirstFrame) 
        {
            ImGui::SetNextWindowSize(ImVec2(400, 150));
            isFirstFrame = false;
        }

        if (ImGui::Begin("Console", &isOn, windowFlags))
        {
            for (unsigned int i = 0; i < messages.size(); i++) 
            {
                ImGui::Text(messages[i].c_str());
            }
        }
        ImGui::End();
    }

	inline static void AddMessage(std::string message) 
    {
        messages.push_back(message);
    }

    inline static void AddMessage(const char* message)
    {
        messages.push_back(message);
    }

    inline static void AddMessage(const char* messagePre, const char* messagePost)
    {
        std::string output = "";
        for (char c = *messagePre; c; c = *++messagePre)
        {
            output.push_back(c);
        }
        for (char c = *messagePost; c; c = *++messagePost)
        {
            output.push_back(c);
        }
        messages.push_back(output);
    }

	//static inline void AddErrorMessage();
	//static inline void AddWarningMessage();
};

#endif //CONSOLE_H