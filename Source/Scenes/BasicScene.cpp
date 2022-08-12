#include "BasicScene.h"
#include "../Drawables/Cube.h"
#include "../Drawables/ObjMesh.h"

BasicScene::BasicScene() : 
    m_cubeObject{glm::vec3(0.0f, 0.0f, -3.0f)}
{
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

    int gameViewWidth, gameViewHeight;
    gameViewWidth = m_masterUI.GetGameWindow()->GetWidth();
    gameViewHeight = m_masterUI.GetGameWindow()->GetHeight();

    // framebuffer configuration
    // -------------------------
    //unsigned int framebuffer;
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    // create a color attachment texture
    //unsigned int textureColorbuffer;
    glGenTextures(1, &m_textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gameViewWidth, gameViewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, gameViewWidth, gameViewHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
}

void BasicScene::Render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.25f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int gameViewWidth, gameViewHeight;
    gameViewWidth = m_masterUI.GetGameWindow()->GetWidth();
    gameViewHeight = m_masterUI.GetGameWindow()->GetHeight();
    glViewport(0, 0, gameViewWidth, gameViewHeight);

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

    glViewport(0, 0, m_width, m_height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_masterUI.SetGameWindowRender(m_framebuffer);
    m_masterUI.PerFrame();

}

void BasicScene::CleanUp()
{
    m_masterUI.CleanUp();
}
