#include "Dimensions.h"
#include "../Dependencies/imgui/imgui.h"

Dimensions::Dimensions(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
	Component{ "Dimensions" },
	m_position{ position },
	m_rotation{ rotation },
	m_scale{ scale },
	m_positionArr{ m_position.x, m_position.y, m_position.z },
	m_rotationArr{ m_rotation.x, m_rotation.y, m_rotation.z },
	m_scaleArr{ m_scale.x, m_scale.y, m_scale.z }
{

}

void Dimensions::Update(float deltaTime)
{

}

void Dimensions::Render(GLSLProgram* prog)
{

}

void Dimensions::DrawUI()
{
    if (m_interfaceOpen) { ImGui::BeginChild("Dimensions", ImVec2(ImGui::GetContentRegionAvail().x, 100), true); }
    else { ImGui::BeginChild("Dimensions", ImVec2(ImGui::GetContentRegionAvail().x, 20), true); }
    if (ImGui::Selectable("Dimensions")) { m_interfaceOpen = !m_interfaceOpen; }
    if (m_interfaceOpen)
    {
        ImGui::DragFloat3("Positon", m_positionArr, 0.05f, -1000.0f, 1000.0f);
        ImGui::DragFloat3("Rotation", m_rotationArr, 1.0f, -360.0f, 360.0f);
        ImGui::DragFloat3("Scale", m_scaleArr, 0.05f, -1000.0f, 1000.0f);
    }
	ImGui::EndChild();

	SetPosition(glm::vec3(m_positionArr[0], m_positionArr[1], m_positionArr[2]));
	SetRotation(glm::vec3(m_rotationArr[0], m_rotationArr[1], m_rotationArr[2]));
	SetScale(glm::vec3(m_scaleArr[0], m_scaleArr[1], m_scaleArr[2]));
}
void Dimensions::Serialization(YAML::Emitter& out)
{
    out << YAML::Key << "dimensions" << YAML::Value;

    out << YAML::BeginMap; // Dimensions Map
    out << YAML::Key << "posX" << YAML::Value << std::to_string(GetPosition().x);
    out << YAML::Key << "posY" << YAML::Value << std::to_string(GetPosition().y);
    out << YAML::Key << "posZ" << YAML::Value << std::to_string(GetPosition().z);
    out << YAML::Key << "rotX" << YAML::Value << std::to_string(GetRotation().x);
    out << YAML::Key << "rotY" << YAML::Value << std::to_string(GetRotation().y);
    out << YAML::Key << "rotZ" << YAML::Value << std::to_string(GetRotation().z);
    out << YAML::Key << "scaX" << YAML::Value << std::to_string(GetScale().x);
    out << YAML::Key << "scaY" << YAML::Value << std::to_string(GetScale().y);
    out << YAML::Key << "scaZ" << YAML::Value << std::to_string(GetScale().z);
    out << YAML::EndMap; // Dimensions Map
}

void Dimensions::SetPosition(glm::vec3 position) { m_position = position; }
void Dimensions::SetRotation(glm::vec3 rotation) { m_rotation = rotation; }
void Dimensions::SetScale(glm::vec3 scale) { m_scale = scale; }

glm::vec3 Dimensions::GetPosition() { return m_position; }
glm::vec3* Dimensions::GetPositionPtr() { return &m_position; }
glm::vec3 Dimensions::GetRotation() { return m_rotation; }
glm::vec3* Dimensions::GetRotationPtr() { return &m_rotation; }
glm::vec3 Dimensions::GetScale() { return m_scale; }
glm::vec3* Dimensions::GetScalePtr() { return &m_scale; }