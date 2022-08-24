#include "Cube.h"
#include "../Dependencies/imgui/imgui.h"
#include "../MasterShaders.h"
#include "../MasterTextures.h"

Cube::Cube(Texture* texture = nullptr, GLfloat size = 1.0f) :
    Component{ "Drawable" },
    m_texture{ texture }
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
    if (m_texture != nullptr) 
    {
        prog->Use();
        m_texture->Bind();
        prog->SetUniform("Texture", m_texture->GetTexture());
    }
    prog->SetUniform("Light.Position", glm::vec4(5.0f, 5.0f, 5.0f, 1.0f));
    prog->SetUniform("Light.La", glm::vec3(0.6f));
    prog->SetUniform("Light.Ld", glm::vec3(0.85f, 0.85f, 0.85f));
    prog->SetUniform("Light.Ls", glm::vec3(1.0f));
    prog->SetUniform("Material.Ka", glm::vec3(0.25f, 0.25f, 0.25f));
    prog->SetUniform("Material.Kd", glm::vec3(0.25f, 0.25f, 0.25f));
    prog->SetUniform("Material.Ks", glm::vec3(0.5f));
    prog->SetUniform("Material.Shininess", 64.0f);
    RenderDrawable();
}

void Cube::DrawUI()
{
    ImGui::BeginChild("Cube", ImVec2(ImGui::GetContentRegionAvail().x, 100), true);
    ImGui::Text("Cube");
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
    ImGui::EndChild();
}
