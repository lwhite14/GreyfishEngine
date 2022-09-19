#ifndef MASTERUI_H
#define MASTERUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/imgui_impl_glfw.h"
#include "../Dependencies/imgui/imgui_impl_opengl3.h"
#include "../Motion.h"
#include "../SceneObjects/Component.h"
#include "../SceneObjects/SceneObject.h"

class MasterUI 
{
private:
    std::string m_openFile;
    std::vector<std::string> m_recentFiles;

    GLFWwindow* m_window;
    ImGuiWindowFlags m_windowFlags;
    ImGuiDockNodeFlags m_dockspaceFlags;
    ImGuiWindowFlags m_windowFlagsChild;
    ImGuiIO* m_io;
    SceneObject* m_selectedSceneObject;

    bool m_hasResized;
    ImVec2 m_size;
    ImVec2 m_offset;
    unsigned int m_gameViewFbo;
    float m_mouseWheel;
    Motion m_camMotion;

    ImVec2 m_gameViewSize;
    int m_sceneObjectIndex;

    
public:
    MasterUI();
    MasterUI(GLFWwindow* window, ImVec2 size);
    void Init();
    void PerFrame(SceneObject* selectedSceneObject, std::vector<SceneObject*>& allSceneObjects);
    void CleanUp();

    void Menu(SceneObject* selectedSceneObject, std::vector<SceneObject*>& allSceneObjects);
    void SceneObjectsWindow(SceneObject* selectedSceneObject, std::vector<SceneObject*>& allSceneObjects);
    void OptionsWindow(SceneObject* selectedSceneObject, std::vector<SceneObject*>& allSceneObjects);
    void GameViewWindow();

    void Save(std::vector<SceneObject*>& allSceneObjects);
    void AddRecentFile(std::string recentFile);
    void LoadRecentFiles();

    GLFWwindow* GetWindow();
    SceneObject* GetSelectedSceneObject();
    ImVec2 GetSize();
    ImVec2 GetOffset();
    unsigned int GetGameViewFBO();
    float GetMouseWheel();
    Motion GetCamMotion();
    ImVec2 GetGameViewSize();

    void SetWindow(GLFWwindow* window);
    void SetSize(ImVec2 size);
    void SetOffset(ImVec2 offset);
    void SetGameViewFBO(unsigned int gameViewFbo);
    void SetMouseWheel(float mouseWheel);
    void SetCamMotion(Motion motion);
    void SetGameViewSize(ImVec2 gameViewSize);
};

#endif //MASTERUI_H