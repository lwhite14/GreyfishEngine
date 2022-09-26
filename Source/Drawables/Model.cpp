#include "Model.h"

#include "../Dependencies/imgui/imgui.h"
#include "../MasterTextures.h"
#include "../MasterObjMeshes.h"

Model::Model() :
    Component{ "Drawable" } 
{

}

Model::Model(ObjMesh* objMesh, SceneObject* associatedObject, Texture* texture) :
    Component{ "Drawable" },
    m_objMesh{ objMesh },
    m_associatedObject{ associatedObject },
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

}

void Model::Update(float deltaTime)
{

}

void Model::Render(GLSLProgram* prog)
{
    prog->Use();
    if (m_texture != nullptr)
    {
        m_texture->Bind();
        prog->SetUniform("Texture", m_texture->GetTexture());
    }
    else
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        prog->SetUniform("Texture", 0);
    }
    prog->SetUniform("Light.Position", glm::vec4(5.0f, 5.0f, 5.0f, 1.0f));
    prog->SetUniform("Light.La", glm::vec3(0.6f));
    prog->SetUniform("Light.Ld", glm::vec3(0.85f, 0.85f, 0.85f));
    prog->SetUniform("Light.Ls", glm::vec3(1.0f));
    prog->SetUniform("Material.Ka", m_matAmbient);
    prog->SetUniform("Material.Kd", m_matDiffuse);
    prog->SetUniform("Material.Ks", m_matSpecular);
    prog->SetUniform("Material.Shininess", m_matShininess);
    if (m_objMesh != nullptr) 
    {
        m_objMesh->RenderDrawable();
    }
}

void Model::DrawUI()
{
    if (m_interfaceOpen) { ImGui::BeginChild("Model", ImVec2(ImGui::GetContentRegionAvail().x, 180), true); }
    else { ImGui::BeginChild("Model", ImVec2(ImGui::GetContentRegionAvail().x, 20), true); }
    if (ImGui::Selectable("Model")) { m_interfaceOpen = !m_interfaceOpen; }
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Button("Remove Component")) { ImGui::CloseCurrentPopup(); m_associatedObject->RemoveComponent(this); }
        ImGui::EndPopup();
    }
    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Right-click to open Component options"); }
    if (m_interfaceOpen)
    {
        std::string meshCombo = "";
        if (m_objMesh == nullptr) { meshCombo = "--No Mesh Selected--"; }
        else { meshCombo = m_objMesh->GetName(); }

        if (ImGui::BeginCombo("ObjMesh", meshCombo.c_str()))
        {
            for (int i = 0; i < MasterObjMeshes::objMeshList.size(); i++)
            {
                if (ImGui::Selectable(MasterObjMeshes::objMeshList[i]->GetName().c_str()))
                {
                    m_objMesh = MasterObjMeshes::objMeshList[i];
                }
            }
            ImGui::EndCombo();
        }

        std::string textureCombo = "";
        if (m_texture == nullptr) { textureCombo = "--No Texture Selected--"; }
        else { textureCombo = m_texture->GetName(); }

        if (ImGui::BeginCombo("Texture", textureCombo.c_str()))
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

void Model::Serialization(YAML::Emitter& out)
{
    out << YAML::BeginMap;

    out << YAML::Key << "model" << YAML::Value;
    out << YAML::BeginMap; // ObjMesh Map

    out << YAML::Key << "objMesh" << YAML::Value << m_objMesh->GetName();
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

    out << YAML::EndMap; // ObjMesh Map
    out << YAML::EndMap;
}

void Model::SetMatAmbient(glm::vec3 matAmbient) { m_matAmbient = matAmbient; m_matAmbientArr[0] = m_matAmbient.x; m_matAmbientArr[1] = m_matAmbient.y; m_matAmbientArr[2] = m_matAmbient.z; }
void Model::SetMatDiffuse(glm::vec3 matDiffuse) { m_matDiffuse = matDiffuse; m_matDiffuseArr[0] = m_matDiffuse.x; m_matDiffuseArr[1] = m_matDiffuse.y; m_matDiffuseArr[2] = m_matDiffuse.z; }
void Model::SetMatSpecular(glm::vec3 matSpecular) { m_matSpecular = matSpecular; m_matSpecularArr[0] = m_matSpecular.x; m_matSpecularArr[1] = m_matSpecular.y; m_matSpecularArr[2] = m_matSpecular.z; }
void Model::SetMatShininess(float matShininess) { m_matShininess = matShininess; m_matShininessArr = m_matShininess; }

void Model::SetAssociatedObject(SceneObject* associatedObject)
{
    m_associatedObject = associatedObject;
}
