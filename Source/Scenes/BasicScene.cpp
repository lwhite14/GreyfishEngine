#include "BasicScene.h"
#include "../Drawables/Cube.h"

BasicScene::BasicScene() : 
    m_tex1{0},
    m_tex2{0}
{
    m_cubeObject.AddComponent(new Cube(1.0f));
}

void BasicScene::Start(GLFWwindow* window)
{
    glEnable(GL_DEPTH_TEST);
    CompileShaders();

    m_view = glm::mat4(1.0f);
    m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, -3.0f));
    m_projection = glm::mat4(1.0f);

    m_tex1 = Texture::LoadTexture("Media/Images/container.jpg");
    m_tex2 = Texture::LoadTexture("Media/Images/awesomeface.png");
}

void BasicScene::CompileShaders()
{
    try
    {
        m_prog.CompileShader("Source/Shaders/Basic.vert");
        m_prog.CompileShader("Source/Shaders/Basic.frag");
        m_prog.Link();
    }
    catch (GLSLProgramException& e)
    {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void BasicScene::Update(GLFWwindow* window, float deltaTime)
{
    m_cubeObject.Update();
}

void BasicScene::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_tex2);

    m_model = glm::mat4(1.0f);
    m_model = glm::rotate(m_model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

    m_prog.Use();
    m_prog.SetUniform("view", m_view);
    m_prog.SetUniform("model", m_model);
    m_prog.SetUniform("projection", m_projection);
    m_prog.SetUniform("texture1", 0);
    m_prog.SetUniform("texture2", 1);

    m_cubeObject.Render();
}

void BasicScene::CleanUp()
{
}
