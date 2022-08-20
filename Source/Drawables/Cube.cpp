#include "Cube.h"
#include "../Dependencies/imgui/imgui.h"

Cube::Cube(GLSLProgram* prog, Texture* texture = nullptr, GLfloat size = 1.0f) :
    m_prog{ prog }, 
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

void Cube::Update() 
{
    // Nothing to update
}

void Cube::Render() 
{
    if (m_texture != nullptr) 
    {
        m_prog->Use();
        m_texture->Bind();
        m_prog->SetUniform("Texture", m_texture->GetTexture());
    }
    RenderDrawable();
}

void Cube::DrawUI() 
{
    ImGui::BeginChild("Cube", ImVec2(ImGui::GetContentRegionAvail().x, 100), true);
    ImGui::Text("Cube");
    ImGui::InputText("##", m_textureName, 64); ImGui::SameLine();
    if (ImGui::Button("Texture")) 
    {
        std::string str = "Media/Images/";
        for (unsigned int i = 0; i < 64; i++)
        {
            if (m_textureName[i] != '\0') { str.push_back(m_textureName[i]); }
            else { break; }
        }
        m_texture = new Texture(str.c_str());
    }
    ImGui::EndChild();
}
