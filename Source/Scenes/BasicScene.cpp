#include "BasicScene.h"

#include "../Drawables/Cube.h"
#include "../Drawables/ObjMesh.h"
#include "../Dependencies/imgui/imgui.h"
#include "../Motion.h"

BasicScene::BasicScene()
{

}

void BasicScene::Start(GLFWwindow* window)
{
    m_masterUI = MasterUI(window, ImVec2(900.0f, 460.0f));
    m_masterUI.Init();

    m_cameraGameView = new CameraGameView(m_width, m_height);

    glEnable(GL_DEPTH_TEST);
    MasterShaders::NewShader("Basic");
    MasterShaders::NewShader("BasicLit");

    m_view = glm::mat4(1.0f);
    m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, 0.0f));

    m_framebuffer = Framebuffer(m_masterUI.GetGameViewSize().x, m_masterUI.GetGameViewSize().y);

    m_cubeObject = new SceneObject("CubeObject", glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    m_cubeObject->AddComponent(new Cube(new Texture("Media/Images/container.jpg"), 1.0f));
}

void BasicScene::Update(GLFWwindow* window, float deltaTime)
{
    m_cubeObject->Update();
    m_cameraGameView->Update(deltaTime, &m_masterUI, m_view);
}

void BasicScene::Render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer.GetFramebuffer());
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_masterUI.GetGameViewSize().x, m_masterUI.GetGameViewSize().y);
  
    m_cubeObject->Render(m_view, m_projection);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, m_width, m_height);
    m_masterUI.SetGameViewFBO(m_framebuffer.GetFramebuffer());
    m_masterUI.PerFrame(m_cubeObject);

}

void BasicScene::CleanUp()
{
    m_masterUI.CleanUp();
}
