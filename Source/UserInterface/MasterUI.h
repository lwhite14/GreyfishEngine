#ifndef MASTERUI_H
#define MASTERUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/imgui_impl_glfw.h"
#include "../Dependencies/imgui/imgui_impl_opengl3.h"
#include "../Motion.h"
#include "../SceneObjects/Component.h"
#include "../SceneObjects/SceneObject.h"
#include "../Framebuffer.h"

class MasterUI 
{
private:
    std::string m_openFile;

    std::vector<std::string> m_recentFiles;

    GLFWwindow* m_window;

    ImGuiWindowFlags m_windowFlags;
    ImGuiWindowFlags m_windowFlagsChild;

    ImGuiDockNodeFlags m_dockspaceFlags;

    ImGuiIO* m_io;

    SceneObject* m_selectedSceneObject;

    ImVec2 m_size;
    ImVec2 m_offset;
    ImVec2 m_sceneViewSize;

    Motion m_camMotion;

    float m_mouseWheel;
    int m_sceneObjectIndex;

    bool m_objectsViewOn;
    bool m_optionsViewOn;
    bool m_sceneViewOn;
    bool m_assetViewOn;

    Framebuffer* m_sceneFramebuffer;
    
public:
    MasterUI();
    MasterUI(GLFWwindow* window, ImVec2 size);
    void Init();
    void PerFrame(std::vector<SceneObject*>& allSceneObjects);
    void CleanUp();

    void Menu(std::vector<SceneObject*>& allSceneObjects);
    void ObjectsWindow(std::vector<SceneObject*>& allSceneObjects);
    void OptionsWindow(std::vector<SceneObject*>& allSceneObjects);
    void SceneWindow();
    void AssetWindow();

    void Save(std::vector<SceneObject*>& allSceneObjects);
    void AddRecentFile(std::string recentFile);
    void SaveRecentFileList();
    void LoadRecentFiles();
    void LoadAsset(nfdchar_t* defaultPath, nfdchar_t* filters, std::string type);

    GLFWwindow* GetWindow();
    SceneObject* GetSelectedSceneObject();
    ImVec2 GetSize();
    ImVec2 GetOffset();
    float GetMouseWheel();
    Motion GetCamMotion();
    ImVec2 GetSceneViewSize();
    Framebuffer* GetSceneFramebuffer();

    void SetWindow(GLFWwindow* window);
    void SetSelectedSceneObject(SceneObject* selectedSceneObject);
    void SetSize(ImVec2 size);
    void SetOffset(ImVec2 offset);
    void SetMouseWheel(float mouseWheel);
    void SetCamMotion(Motion motion);
    void SetSceneViewSize(ImVec2 gameViewSize);
    void SetSceneFramebuffer(Framebuffer* sceneFramebuffer);
};

#endif //MASTERUI_H