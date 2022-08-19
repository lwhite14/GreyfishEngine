#include "BasicScene.h"

#include "../Drawables/Cube.h"
#include "../Drawables/ObjMesh.h"
#include "../Dependencies/imgui/imgui.h"
#include "../Motion.h"

BasicScene::BasicScene() : 
    m_cubeObject{glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)}
{
    m_cubeObject.AddComponent(new Cube(1.0f));
}

void BasicScene::Start(GLFWwindow* window)
{
    m_masterUI = MasterUI(window, ImVec2(900.0f, 460.0f));
    m_masterUI.Init();

    m_cameraGameView = new CameraGameView(m_width, m_height);

    glEnable(GL_DEPTH_TEST);
    CompileShaders();

    m_view = glm::mat4(1.0f);
    m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, 0.0f));

    int gameViewWidth = m_masterUI.GetGameViewSize().x;
    int gameViewHeight = m_masterUI.GetGameViewSize().y;
    m_framebuffer = Framebuffer(gameViewWidth, gameViewHeight);
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
    m_cameraGameView->Update(deltaTime, &m_masterUI, m_view);
}

void BasicScene::Render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer.GetFramebuffer());
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int gameViewWidth, gameViewHeight;
    gameViewWidth = m_masterUI.GetGameViewSize().x;
    gameViewHeight = m_masterUI.GetGameViewSize().y;
    glViewport(0, 0, gameViewWidth, gameViewHeight);

    glm::mat4 cubeObjectModel = glm::mat4(1.0f);

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

    glViewport(0, 0, m_width, m_height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_masterUI.SetGameViewFBO(m_framebuffer.GetFramebuffer());
    m_masterUI.PerFrame(m_cubeObject.GetAllComponents());

}

void BasicScene::CleanUp()
{
    m_masterUI.CleanUp();
}
