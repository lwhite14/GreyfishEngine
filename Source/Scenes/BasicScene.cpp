#include "BasicScene.h"

#include "../Drawables/Cube.h"
#include "../Drawables/ObjMesh.h"
#include "../Dependencies/imgui/imgui.h"
#include "../Motion.h"
#include "../SceneObjects/Spinner.h"
#include "../MasterObjMeshes.h"
#include "../Drawables/Model.h"
#include "../GreyfishParsing.h"

BasicScene::BasicScene()
{

}

void BasicScene::Start(GLFWwindow* window)
{
    m_masterUI = MasterUI(window, ImVec2(m_width, m_height));
    m_masterUI.Init();

    m_cameraGameView = new CameraGameView(1920, 1080);

    glEnable(GL_DEPTH_TEST);

    m_framebuffer = new Framebuffer(m_width, m_height);

    GreyfishParsing::LoadAssets();

    m_view = glm::mat4(1.0f);
    m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, 0.0f));

    Console::AddMessage("---- INITIALIZATION COMPLETED ----");
}

void BasicScene::Update(GLFWwindow* window, float deltaTime)
{
    for (unsigned int i = 0; i < m_sceneObjects.size(); i++) { m_sceneObjects[i]->Update(deltaTime); }
    m_cameraGameView->Update(deltaTime, &m_masterUI, m_view);
}

void BasicScene::Render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->GetFramebuffer());
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_framebuffer->GetGameViewWidth(), m_framebuffer->GetGameViewHeight());
  
    for (unsigned int i = 0; i < m_sceneObjects.size(); i++) { m_sceneObjects[i]->Render(m_view, m_projection); }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, m_width, m_height);
    m_masterUI.SetSceneFramebuffer(m_framebuffer);
    m_masterUI.PerFrame(m_sceneObjects);

    Resize(m_masterUI.GetSceneViewSize().x, m_masterUI.GetSceneViewSize().y);
}

void BasicScene::CleanUp()
{
    m_masterUI.CleanUp();
}