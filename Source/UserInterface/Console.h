#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>
#include <string>
#include "ConsoleMessage.h"
#include "../Dependencies/imgui/imgui.h"

class Console 
{
private:
	inline static std::vector<ConsoleMessage> messages;

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
        //windowFlags |= ImGuiWindowFlags_NoCollapse;
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
                if (messages[i].GetMessageType() == 0) { ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255)); }
                else if (messages[i].GetMessageType() == 1) { ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255)); }
                else if (messages[i].GetMessageType() == 2) { ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255)); }
                else { ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255)); }
                ImGui::Text(messages[i].GetMessage().c_str());
                ImGui::PopStyleColor();
            }
        }
        ImGui::End();
    }

	inline static void AddMessage(std::string message) 
    {
        messages.push_back(ConsoleMessage(message.c_str(), 0));
    }

    inline static void AddMessage(const char* message)
    {
        messages.push_back(ConsoleMessage(message, 0));
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
        messages.push_back(ConsoleMessage(output, 0));
    }

    inline static void AddWarningMessage(std::string message)
    {
        messages.push_back(ConsoleMessage(message.c_str(), 1));
    }

    inline static void AddWarningMessage(const char* message)
    {
        messages.push_back(ConsoleMessage(message, 1));
    }

    inline static void AddWarningMessage(const char* messagePre, const char* messagePost)
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
        messages.push_back(ConsoleMessage(output, 1));
    }

    inline static void AddErrorMessage(std::string message)
    {
        messages.push_back(ConsoleMessage(message.c_str(), 2));
    }

    inline static void AddErrorMessage(const char* message)
    {
        messages.push_back(ConsoleMessage(message, 2));
    }

    inline static void AddErrorMessage(const char* messagePre, const char* messagePost)
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
        messages.push_back(ConsoleMessage(output, 2));
    }
};

#endif //CONSOLE_H