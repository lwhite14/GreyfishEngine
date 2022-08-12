#include "BasicScene.h"
#include "../Drawables/Cube.h"
#include "../Drawables/ObjMesh.h"

BasicScene::BasicScene() : 
    m_cubeObject{glm::vec3(0.0f, 0.0f, -3.0f)},
    m_tex1{0},
    m_tex2{0}
{
    //m_cubeObject.AddComponent(ObjMesh::Load("Media/Models/car.obj"));
    m_cubeObject.AddComponent(new Cube(1.0f));
}

void BasicScene::Start(GLFWwindow* window)
{
    m_masterUI = MasterUI();
    m_masterUI.Init(window);

    glEnable(GL_DEPTH_TEST);
    CompileShaders();

    m_view = glm::mat4(1.0f);
    m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, 0.0f));

    m_tex1 = Texture::LoadTexture("Media/Images/container.jpg");
    m_tex2 = Texture::LoadTexture("Media/Images/awesomeface.png");
}

void BasicScene::CompileShaders()
{
    try
    {
        m_prog.CompileShader("Source/Shaders/BasicLit.vert");
        m_prog.CompileShader("Source/Shaders/BasicLit.frag");
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
    m_masterUI.PerFrame();
}

void BasicScene::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_tex2);

    glm::mat4 cubeObjectModel = glm::mat4(1.0f);
    cubeObjectModel = glm::rotate(cubeObjectModel, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

    m_prog.Use();
    m_prog.SetUniform("Light.Position", glm::vec4(5.0f, 5.0f, 5.0f, 1.0f));
    m_prog.SetUniform("Light.La", glm::vec3(0.6f));
    m_prog.SetUniform("Light.Ld", glm::vec3(0.85f, 0.2f, 0.2f));
    m_prog.SetUniform("Light.Ls", glm::vec3(1.0f));
    m_prog.SetUniform("Material.Ka", glm::vec3(0.5f, 0.25f, 0.25f));
    m_prog.SetUniform("Material.Kd", glm::vec3(0.5f, 0.25f, 0.25f));
    m_prog.SetUniform("Material.Ks", glm::vec3(0.5f));
    m_prog.SetUniform("Material.Shininess", 64.0f);

    m_cubeObject.SetModel(&cubeObjectModel);
    m_cubeObject.Render(&m_prog, m_view, m_projection);
}

void BasicScene::CleanUp()
{
    m_masterUI.CleanUp();
}
