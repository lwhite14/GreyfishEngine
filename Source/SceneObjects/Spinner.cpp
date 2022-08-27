#include "Spinner.h"

#include "../Dependencies/imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

Spinner::Spinner(glm::mat4* model, SceneObject* associatedObject) :
    Component{ "Spinner" },
    m_associatedObject{ associatedObject },
    m_model{ model },
    m_speed{ 0.05f }
{

}

void Spinner::Update(float deltaTime)
{
    *m_model = glm::rotate(*m_model, (float)glfwGetTime() * m_speed * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Spinner::Render(GLSLProgram* prog)
{
	// Not drawable
}

void Spinner::DrawUI()
{
    ImGui::BeginChild("Spinner", ImVec2(ImGui::GetContentRegionAvail().x, 100), true);
    ImGui::Selectable("Spinner");
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Button("Remove Component")) { ImGui::CloseCurrentPopup(); m_associatedObject->RemoveComponent(this); }
        ImGui::EndPopup();
    }
    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Right-click to open component options"); }
    ImGui::DragFloat("Spin Speed", &m_speed, 0.005f);
    ImGui::EndChild();
}
