#ifndef IMGUIUTILS_H
#define IMGUIUTILS_H

#include "../Dependencies/imgui/imgui.h"

namespace ImGui 
{
	inline static void SetUpStyle() 
	{
        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowPadding = ImVec2(4.0f, 4.0f);
        style.FramePadding = ImVec2(4.0f, 4.0f);
        style.CellPadding = ImVec2(4.0f, 4.0f);
        style.ItemSpacing = ImVec2(4.0f, 4.0f);
        style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
        style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
        style.IndentSpacing = 10.0f;
        style.ScrollbarSize = 10.0f;
        style.GrabMinSize = 10.0f;

        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupBorderSize = 0.0f;
        style.FrameBorderSize = 0.0f;
        style.TabBorderSize = 1.0f;

        style.WindowRounding = 6.0f;
        style.ChildRounding = 6.0f;
        style.FrameRounding = 6.0f;
        style.PopupRounding = 6.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 6.0f;
        style.LogSliderDeadzone = 6.0f;
        style.TabRounding = 6.0f;

        style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_Left;
        style.ColorButtonPosition = ImGuiDir_Right;
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
        style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);

        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.36f, 0.51f, 0.15f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.36f, 0.51f, 0.15f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.55f, 0.78f, 0.21f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.36f, 0.51f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.51f, 0.73f, 0.10f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.78f, 0.21f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.64f, 0.91f, 0.13f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.91f, 0.13f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.36f, 0.51f, 0.15f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.51f, 0.73f, 0.10f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.78f, 0.21f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.36f, 0.51f, 0.15f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.71f, 0.09f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.51f, 0.73f, 0.10f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.64f, 0.91f, 0.13f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.55f, 0.78f, 0.21f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.64f, 0.91f, 0.13f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.78f, 0.21f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.36f, 0.51f, 0.15f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.51f, 0.73f, 0.10f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.55f, 0.78f, 0.21f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.36f, 0.51f, 0.15f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.55f, 0.78f, 0.21f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.55f, 0.78f, 0.21f, 1.00f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.70f, 0.90f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.60f, 1.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.27f, 0.27f, 0.38f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.45f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}
}

#endif //IMGUIUTILS_H