#include "Cube.h"
#include "../Dependencies/imgui/imgui.h"
#include "../MasterShaders.h"
#include "../MasterTextures.h"

Cube::Cube(Texture* texture, GLfloat size, SceneObject* sceneObject) :
    Component{ "Drawable" },
    m_associatedObject{ sceneObject },
    m_texture{ texture },
    m_matAmbient{ glm::vec3(0.25f) },
    m_matDiffuse{ glm::vec3(0.25f) },
    m_matSpecular{ glm::vec3(0.5f) },
    m_matShininess{ 64.0f },
    m_matAmbientArr{ m_matAmbient.x, m_matAmbient.y, m_matAmbient.z },
    m_matDiffuseArr{ m_matDiffuse.x, m_matDiffuse.y, m_matDiffuse.z },
    m_matSpecularArr{ m_matSpecular.x, m_matSpecular.y, m_matSpecular.z },
    m_matShininessArr{ m_matShininess }
{
    GLfloat side = size / 2.0f;

    std::vector<GLfloat> points = 
    {
       -side, -side, side, side, -side, side, side,  side, side,  -side,  side, side,
        side, -side, side, side, -side, -side, side,  side, -side, side,  side, side,
        -side, -side, -side, -side,  side, -side, side,  side, -side, side, -side, -side,
        -side, -side, side, -side,  side, side, -side,  side, -side, -side, -side, -side,
        -side, -side, side, -side, -side, -side, side, -side, -side, side, -side, side,
        -side,  side, side, side,  side, side, side,  side, -side, -side,  side, -side
    };

    std::vector<GLfloat> normal =
    {
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    std::vector<GLfloat> texCoords = 
    {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    std::vector<GLuint> indices = 
    {
        0,1,2,0,2,3,
        4,5,6,4,6,7,
        8,9,10,8,10,11,
        12,13,14,12,14,15,
        16,17,18,16,18,19,
        20,21,22,20,22,23
    };

    InitBuffers(&indices, &points, &normal, &texCoords);
}

void Cube::Update(float deltaTime)
{
    // Nothing to update
}

void Cube::Render(GLSLProgram* prog)
{
    prog->Use();
    if (m_texture != nullptr) 
    {
        m_texture->Bind();
        prog->SetUniform("Texture", m_texture->GetTexture());
    }
    prog->SetUniform("Light.Position", glm::vec4(5.0f, 5.0f, 5.0f, 1.0f));
    prog->SetUniform("Light.La", glm::vec3(0.6f));
    prog->SetUniform("Light.Ld", glm::vec3(0.85f, 0.85f, 0.85f));
    prog->SetUniform("Light.Ls", glm::vec3(1.0f));
    prog->SetUniform("Material.Ka", m_matAmbient);
    prog->SetUniform("Material.Kd", m_matDiffuse);
    prog->SetUniform("Material.Ks", m_matSpecular);
    prog->SetUniform("Material.Shininess", m_matShininess);
    RenderDrawable();
}

void Cube::DrawUI()
{
    if (m_interfaceOpen) { ImGui::BeginChild("Cube", ImVec2(ImGui::GetContentRegionAvail().x, 160), true); }
    else { ImGui::BeginChild("Cube", ImVec2(ImGui::GetContentRegionAvail().x, 20), true); }
    if (ImGui::Selectable("Cube")) { m_interfaceOpen = !m_interfaceOpen; }
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Button("Remove Component")) { ImGui::CloseCurrentPopup(); m_associatedObject->RemoveComponent(this); }
        ImGui::EndPopup();
    }
    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Right-click to open Component options"); }
    if (m_interfaceOpen)
    {
        if (ImGui::BeginCombo("Texture", m_texture->GetName().c_str()))
        {
            for (int i = 0; i < MasterTextures::textureList.size(); i++)
            {
                if (ImGui::Selectable(MasterTextures::textureList[i]->GetName().c_str()))
                {
                    m_texture = MasterTextures::textureList[i];
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Text("Material");
        ImGui::DragFloat3("Ambient", m_matAmbientArr, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat3("Diffuse", m_matDiffuseArr, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat3("Specular", m_matSpecularArr, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Shininess", &m_matShininessArr, 1.0f, 0.0f, 128.0f);
    }
    ImGui::EndChild();

    SetMatAmbient(glm::vec3(m_matAmbientArr[0], m_matAmbientArr[1], m_matAmbientArr[2]));
    SetMatDiffuse(glm::vec3(m_matDiffuseArr[0], m_matDiffuseArr[1], m_matDiffuseArr[2]));
    SetMatSpecular(glm::vec3(m_matSpecularArr[0], m_matSpecularArr[1], m_matSpecularArr[2]));
    SetMatShininess(m_matShininessArr);
}

void Cube::Serialization(YAML::Emitter& out)
{
    out << YAML::BeginMap;

    out << YAML::Key << "cube" << YAML::Value;
    out << YAML::BeginMap; // Cube Map

    out << YAML::Key << "texture" << YAML::Value << m_texture->GetName();
    out << YAML::Key << "matAmbientX" << YAML::Value << std::to_string(m_matAmbient.x);
    out << YAML::Key << "matAmbientY" << YAML::Value << std::to_string(m_matAmbient.y);
    out << YAML::Key << "matAmbientZ" << YAML::Value << std::to_string(m_matAmbient.z);
    out << YAML::Key << "matDiffuseX" << YAML::Value << std::to_string(m_matDiffuse.x);
    out << YAML::Key << "matDiffuseY" << YAML::Value << std::to_string(m_matDiffuse.y);
    out << YAML::Key << "matDiffuseZ" << YAML::Value << std::to_string(m_matDiffuse.z);
    out << YAML::Key << "matSpecularX" << YAML::Value << std::to_string(m_matSpecular.x);
    out << YAML::Key << "matSpecularY" << YAML::Value << std::to_string(m_matSpecular.y);
    out << YAML::Key << "matSpecularZ" << YAML::Value << std::to_string(m_matSpecular.z);
    out << YAML::Key << "matShininess" << YAML::Value << std::to_string(m_matShininess);

    out << YAML::EndMap; // Cube Map
    out << YAML::EndMap;
}

void Cube::SetMatAmbient(glm::vec3 matAmbient) { m_matAmbient = matAmbient; m_matAmbientArr[0] = m_matAmbient.x; m_matAmbientArr[1] = m_matAmbient.y; m_matAmbientArr[2] = m_matAmbient.z; }
void Cube::SetMatDiffuse(glm::vec3 matDiffuse) { m_matDiffuse = matDiffuse; m_matDiffuseArr[0] = m_matDiffuse.x; m_matDiffuseArr[1] = m_matDiffuse.y; m_matDiffuseArr[2] = m_matDiffuse.z; }
void Cube::SetMatSpecular(glm::vec3 matSpecular) { m_matSpecular = matSpecular; m_matSpecularArr[0] = m_matSpecular.x; m_matSpecularArr[1] = m_matSpecular.y; m_matSpecularArr[2] = m_matSpecular.z; }
void Cube::SetMatShininess(float matShininess) { m_matShininess = matShininess; m_matShininessArr = m_matShininess; }
