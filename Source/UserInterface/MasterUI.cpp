#include "MasterUI.h"

#include <yaml-cpp/yaml.h>
#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/imgui_impl_glfw.h"
#include "../Dependencies/imgui/imgui_impl_opengl3.h"
#include "../Drawables/Cube.h"
#include "../Drawables/ObjMesh.h"
#include "../SceneObjects/Spinner.h"
#include "../MasterShaders.h"
#include "../MasterTextures.h"
#include "../SceneParser.h"
#include "ImGuiUtils.h"
#include "Console.h"

MasterUI::MasterUI() { }

MasterUI::MasterUI(GLFWwindow* window, ImVec2 size) :
    m_openFile { "" },
    m_window{ window },
    m_size{ size },
    m_windowFlags{ 0 }, m_offset{ 0, 0 },
    m_gameViewFbo{ 0 },
    m_hasResized{ false },
    m_mouseWheel{ 0.0f },
    m_camMotion{ false, false, false, false },
    m_sceneObjectIndex{ 0 }
{
    
}

void MasterUI::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    m_io = &ImGui::GetIO();
    m_io->Fonts->AddFontFromFileTTF("Settings/Fonts/Roboto-Bold.ttf", 12);
    ImGui::SetUpStyle();

    m_windowFlags |= ImGuiWindowFlags_NoResize;
    m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
    m_windowFlags |= ImGuiWindowFlags_NoScrollbar;
    m_windowFlags |= ImGuiWindowFlags_MenuBar;
    m_windowFlags |= ImGuiWindowFlags_NoMove;
    m_windowFlags |= ImGuiWindowFlags_NoCollapse;
    //m_windowFlags |= ImGuiWindowFlags_NoNav;
    //m_windowFlags |= ImGuiWindowFlags_NoBackground;
    m_windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    //m_windowFlags |= ImGuiWindowFlags_NoDocking;
    //m_windowFlags |= ImGuiWindowFlags_UnsavedDocument;
}

void MasterUI::PerFrame(SceneObject* selectedSceneObject, std::vector<SceneObject*>& allSceneObjects)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    m_selectedSceneObject = selectedSceneObject;

    /////////////
    // Windows //
    /////////////

    // Workspace Window
    if (!m_hasResized) 
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(m_size);
        m_hasResized = true;
    }

    ImGui::Begin("Workspace", NULL, m_windowFlags);
    Menu(selectedSceneObject, allSceneObjects);
    SceneObjectsWindow(selectedSceneObject, allSceneObjects);
    OptionsWindow(selectedSceneObject, allSceneObjects);
    GameViewWindow();
    FramerateWindow();
    if (Console::isOn) { Console::PerFrame(); }
    ImGui::End();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void MasterUI::FramerateWindow() 
{
    bool p_open = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetContentRegionAvail().x - 80, 30));
    ImGui::SetNextWindowSize(ImVec2(75, 20));
    ImGui::SetNextWindowViewport(viewport->ID);
    if (ImGui::Begin("Example: Simple overlay", &p_open, window_flags))
    {
        ImGui::Text("FPS: %.1f", m_io->Framerate);
    }
    ImGui::End();
}

void MasterUI::SceneObjectsWindow(SceneObject* selectedSceneObject, std::vector<SceneObject*>& allSceneObjects)
{
    // SceneObjects Child
    ImGui::BeginChild("##", ImVec2(m_size.x * 0.15f, ImGui::GetContentRegionAvail().y), true);
    ImGui::Text("SceneObjects in Scene:");
    ImGui::Spacing();
    //m_sceneObjectIndex = 0;
    for (unsigned int i = 0; i < allSceneObjects.size(); i++)
    {
        const bool isSelected = (m_sceneObjectIndex == i);
        if (ImGui::Selectable(allSceneObjects[i]->GetName().c_str(), isSelected)) { m_selectedSceneObject = allSceneObjects[i]; m_sceneObjectIndex = i; }
        if (isSelected) { ImGui::SetItemDefaultFocus(); }

        if (allSceneObjects[i] == selectedSceneObject)
        {
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::Button("Remove SceneObject"))
                {
                    ImGui::CloseCurrentPopup();
                    // Remove SceneObject

                    int indexToRemove = -1;
                    for (unsigned int j = 0; j < allSceneObjects.size(); j++)
                    {
                        if (allSceneObjects[j] == selectedSceneObject)
                        {
                            indexToRemove = j;
                        }
                    }
                    selectedSceneObject = nullptr;
                    m_selectedSceneObject = nullptr;
                    allSceneObjects.erase(allSceneObjects.begin() + indexToRemove);
                }
                ImGui::EndPopup();
            }
            if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Right-click to open SceneObject options"); }
        }
    }
    if (ImGui::Button("  +  ", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
    {
        bool needToRename = false;
        for (unsigned int i = 0; i < allSceneObjects.size(); i++) { if (allSceneObjects[i]->GetName() == "?") { needToRename = true; } }
        if (needToRename)
        {
            ImGui::OpenPopup("NeedToRename");
        }
        else
        {
            allSceneObjects.push_back(new SceneObject("?", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), MasterShaders::shaderList[0]));
            selectedSceneObject = allSceneObjects[allSceneObjects.size() - 1];
            m_selectedSceneObject = allSceneObjects[allSceneObjects.size() - 1];
            m_sceneObjectIndex = allSceneObjects.size() - 1;
        }
    }
    if (ImGui::BeginPopup("NeedToRename"))
    {
        ImGui::Text("Rename your new SceneObject first!");
        ImGui::EndPopup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
}

void MasterUI::OptionsWindow(SceneObject* selectedSceneObject, std::vector<SceneObject*>& allSceneObjects)
{
    // Options Child
    ImGui::BeginChild("Game View Options", ImVec2(m_size.x * 0.25f, ImGui::GetContentRegionAvail().y), true);
    if (selectedSceneObject != nullptr)
    {
        selectedSceneObject->DrawHeaderUI();
        std::vector<Component*> components = selectedSceneObject->GetAllComponents();
        for (unsigned int i = 0; i < components.size(); i++) { components[i]->DrawUI(); }
        if (ImGui::Button("  +  ", ImVec2(ImGui::GetContentRegionAvail().x, 0))) { ImGui::OpenPopup("AddComponentPopup"); }
        ImGui::SameLine();
        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            ImVec2 size = ImVec2(100, 0);
            if (ImGui::Selectable("Cube", false, 0, size)) { selectedSceneObject->AddComponent(new Cube(MasterTextures::textureList[0], 1.0f, selectedSceneObject)); }
            if (ImGui::Selectable("ObjMesh", false, 0, size)) { selectedSceneObject->AddComponent(ObjMesh::Load("Media/Models/suzanne.obj", selectedSceneObject, MasterTextures::textureList[0])); }
            if (ImGui::Selectable("Spinner", false, 0, size)) { selectedSceneObject->AddComponent(new Spinner(selectedSceneObject->GetModelPtr(), selectedSceneObject)); }
            ImGui::EndPopup();
        }
    }
    ImGui::EndChild();
    ImGui::SameLine();
}

void MasterUI::GameViewWindow()
{
    // Game View Child
    ImGui::BeginChild("Game View", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
    m_gameViewSize = ImGui::GetContentRegionAvail();
    ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_gameViewFbo), ImVec2(m_gameViewSize.x, m_gameViewSize.y), ImVec2(0, 1), ImVec2(1, 0), 0);
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            m_offset.x += ImGui::GetIO().MouseDelta.x;
            m_offset.y += ImGui::GetIO().MouseDelta.y;
        }
        m_mouseWheel = m_io->MouseWheel;
        if (ImGui::IsKeyDown(ImGuiKey_A) || ImGui::IsKeyDown(ImGuiKey_D) || ImGui::IsKeyDown(ImGuiKey_S) || ImGui::IsKeyDown(ImGuiKey_W) || ImGui::IsKeyDown(ImGuiKey_LeftArrow) || ImGui::IsKeyDown(ImGuiKey_RightArrow) || ImGui::IsKeyDown(ImGuiKey_UpArrow) || ImGui::IsKeyDown(ImGuiKey_DownArrow))
        {
            if (ImGui::IsKeyDown(ImGuiKey_A) || ImGui::IsKeyDown(ImGuiKey_LeftArrow)) { m_camMotion.Left = true; }
            else { m_camMotion.Left = false; }
            if (ImGui::IsKeyDown(ImGuiKey_D) || ImGui::IsKeyDown(ImGuiKey_RightArrow)) { m_camMotion.Right = true; }
            else { m_camMotion.Right = false; }
            if (ImGui::IsKeyDown(ImGuiKey_S) || ImGui::IsKeyDown(ImGuiKey_DownArrow)) { m_camMotion.Down = true; }
            else { m_camMotion.Down = false; }
            if (ImGui::IsKeyDown(ImGuiKey_W) || ImGui::IsKeyDown(ImGuiKey_UpArrow)) { m_camMotion.Up = true; }
            else { m_camMotion.Up = false; }
        }
        else { m_camMotion.Left = false; m_camMotion.Right = false; m_camMotion.Down = false; m_camMotion.Up = false; }
    }
    else
    {
        m_mouseWheel = 0;
        m_camMotion.Left = false;
        m_camMotion.Right = false;
        m_camMotion.Down = false;
        m_camMotion.Up = false;
    }
    ImGui::EndChild();
}

void MasterUI::Menu(SceneObject* selectedSceneObject, std::vector<SceneObject*>& allSceneObjects)
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) 
            {
                Console::AddMessage("Scene Dialog: New Scene");
                allSceneObjects = std::vector<SceneObject*>();
                m_sceneObjectIndex = -1;
                selectedSceneObject = nullptr;
                m_selectedSceneObject = nullptr;
            }
            if (ImGui::MenuItem("Open")) 
            {
                nfdchar_t* outPath = NULL;
                nfdchar_t filters[] = "yaml";
                nfdchar_t defaultPath[] = "Media\Scenes\\0";
                nfdresult_t result = NFD_OpenDialog(filters, defaultPath, &outPath);

                if (result == NFD_OKAY)
                {
                    Console::AddMessage("Scene Dialog: Success!");
                    bool isFileEx = false;
                    std::string ex = "";
                    std::string filePath = "";
                    for (char c = *outPath; c; c = *++outPath)
                    {
                        filePath.push_back(c);
                        if (isFileEx) 
                        {
                            ex.push_back(c);
                        }
                        if (c == '.') 
                        {
                            isFileEx = true;
                        }
                    }
                    if (ex == "yaml") 
                    {
                        Console::AddMessage("Scene Dialog: " + filePath);
                        bool success = SceneParser::IsValidFile(filePath);
                        if (success) 
                        {
                            allSceneObjects = std::vector<SceneObject*>();
                            allSceneObjects = SceneParser::LoadFileIntoSceneObjects(filePath); 
                            m_sceneObjectIndex = -1;
                            selectedSceneObject = nullptr;
                            m_selectedSceneObject = nullptr;
                            m_openFile = filePath;
                        }
                        else 
                        {
                            Console::AddMessage("Scene Dialog: Not a valid yaml file.");
                        }
                    }
                    else 
                    {
                        Console::AddMessage("Scene Dialog: Wrong file extension.");
                    }
                }
                else if (result == NFD_CANCEL)
                {
                    Console::AddMessage("Scene Dialog: User pressed cancel.");
                }
                else
                {
                    Console::AddMessage("Scene Dialog: ", NFD_GetError());
                }
            }
            if (ImGui::MenuItem("Open Recent")) 
            {
                Console::AddMessage("Scene Dialog: Open Recent Scene.");
            }
            if (ImGui::MenuItem("Save")) 
            {
                Console::AddMessage(m_openFile);
                if (m_openFile != "") 
                {
                    SceneParser::SaveSceneObjectsIntoFile(m_openFile, allSceneObjects);
                }
                else 
                {
                    Console::AddMessage("Scene Dialog: No Open File.");
                    Save(allSceneObjects);
                }
            }
            if (ImGui::MenuItem("Save As")) 
            {
                Save(allSceneObjects);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) 
        {
            if (ImGui::MenuItem("Console")) 
            {
                Console::isOn = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void MasterUI::Save(std::vector<SceneObject*>& allSceneObjects)
{
    nfdchar_t* outPath = NULL;
    nfdchar_t filters[] = "yaml";
    nfdchar_t defaultPath[] = "Media\Scenes\\0";
    nfdresult_t result = NFD_SaveDialog(filters, defaultPath, &outPath);

    if (result == NFD_OKAY)
    {
        Console::AddMessage("Scene Dialog: Success!");
        bool isFileEx = false;
        std::string filePath = "";
        for (char c = *outPath; c; c = *++outPath)
        {
            if (c == '.')
            {
                isFileEx = true;
            }
            if (!isFileEx)
            {
                filePath.push_back(c);
            }
        }
        filePath.push_back('.');
        filePath.push_back('y');
        filePath.push_back('a');
        filePath.push_back('m');
        filePath.push_back('l');
        filePath.push_back('\0');
        SceneParser::SaveSceneObjectsIntoFile(filePath, allSceneObjects);
        m_openFile = filePath;
    }
    else if (result == NFD_CANCEL) 
    {
        Console::AddMessage("Scene Dialog: User pressed cancel!");
    }
    else 
    {
        Console::AddMessage("Scene Dialog: ", NFD_GetError());
    }
}

void MasterUI::CleanUp() 
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

GLFWwindow* MasterUI::GetWindow() { return m_window; }
SceneObject* MasterUI::GetSelectedSceneObject() { return m_selectedSceneObject; }
ImVec2 MasterUI::GetSize() { return m_size; }
ImVec2 MasterUI::GetOffset() { return m_offset; }
unsigned int MasterUI::GetGameViewFBO() { return m_gameViewFbo; }
float MasterUI::GetMouseWheel() { return m_mouseWheel; }
Motion MasterUI::GetCamMotion() { return m_camMotion; }
ImVec2 MasterUI::GetGameViewSize() { return m_gameViewSize; }

void MasterUI::SetWindow(GLFWwindow* window) { m_window = window; }
void MasterUI::SetSize(ImVec2 size) { m_size = size; }
void MasterUI::SetOffset(ImVec2 offset) { m_offset = offset; }
void MasterUI::SetGameViewFBO(unsigned int gameViewFbo) { m_gameViewFbo = gameViewFbo; }
void MasterUI::SetMouseWheel(float mouseWheel) { m_mouseWheel = mouseWheel; }
void MasterUI::SetCamMotion(Motion motion) { m_camMotion = motion; }
void MasterUI::SetGameViewSize(ImVec2 gameViewSize) { m_gameViewSize = gameViewSize; }