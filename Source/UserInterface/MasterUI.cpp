#include "MasterUI.h"

#include <yaml-cpp/yaml.h>
#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/imgui_impl_glfw.h"
#include "../Dependencies/imgui/imgui_impl_opengl3.h"
#include "../Drawables/Cube.h"
#include "../SceneObjects/Spinner.h"
#include "../MasterShaders.h"
#include "../MasterTextures.h"
#include "../GreyfishParsing.h"
#include "ImGuiUtils.h"
#include "Console.h"
#include "../MasterObjMeshes.h"
#include "../Drawables/Model.h"

MasterUI::MasterUI() { }

MasterUI::MasterUI(GLFWwindow* window, ImVec2 size) :
    m_selectedSceneObject{ nullptr },
    m_openFile { "" },
    m_recentFiles { std::vector<std::string>(5) },
    m_window{ window },
    m_size{ size },
    m_windowFlags{ 0 }, 
    m_offset{ 0, 0 },
    m_mouseWheel{ 0.0f },
    m_camMotion{ false, false, false, false },
    m_sceneObjectIndex{ -1 },
    m_objectsViewOn{ true },
    m_optionsViewOn{ true },
    m_sceneViewOn{ true },
    m_assetViewOn{ true }
{
    
}

void MasterUI::Init()
{
    LoadRecentFiles();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    m_io = &ImGui::GetIO();
    m_io->Fonts->AddFontFromFileTTF("Settings/EngineAssets/Fonts/Roboto-Bold.ttf", 12);
    m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::SetUpStyle();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    m_windowFlags |= ImGuiWindowFlags_NoResize;
    m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
    //m_windowFlags |= ImGuiWindowFlags_NoScrollbar;
    m_windowFlags |= ImGuiWindowFlags_MenuBar;
    m_windowFlags |= ImGuiWindowFlags_NoMove;
    m_windowFlags |= ImGuiWindowFlags_NoCollapse;
    //m_windowFlags |= ImGuiWindowFlags_NoNav;
    //m_windowFlags |= ImGuiWindowFlags_NoBackground;
    m_windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    m_windowFlags |= ImGuiWindowFlags_NoDocking;
    //m_windowFlags |= ImGuiWindowFlags_UnsavedDocument;
    m_windowFlags |= ImGuiWindowFlags_NoNavFocus;

    m_dockspaceFlags = ImGuiDockNodeFlags_None;

    m_windowFlagsChild = ImGuiModFlags_None;
    //m_windowFlagsChild |= ImGuiWindowFlags_NoResize;
    //m_windowFlagsChild |= ImGuiWindowFlags_NoTitleBar;
    //m_windowFlagsChild |= ImGuiWindowFlags_NoScrollbar;
    //m_windowFlagsChild |= ImGuiWindowFlags_MenuBar;
    //m_windowFlagsChild |= ImGuiWindowFlags_NoMove;
    m_windowFlagsChild |= ImGuiWindowFlags_NoCollapse;
    //m_windowFlagsChild |= ImGuiWindowFlags_NoNav;
    //m_windowFlagsChild |= ImGuiWindowFlags_NoBackground;
    //m_windowFlagsChild |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    //m_windowFlagsChild |= ImGuiWindowFlags_NoDocking;
    //m_windowFlagsChild |= ImGuiWindowFlags_UnsavedDocument;
}

void MasterUI::PerFrame(std::vector<SceneObject*>& allSceneObjects)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    /////////////
    // Windows //
    /////////////


    // Workspace Window
    ImGui::Begin("Workspace", NULL, m_windowFlags);
    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), m_dockspaceFlags);
    Menu(allSceneObjects);
    ImGui::End();

    if (m_objectsViewOn) { ObjectsWindow(allSceneObjects); }
    if (m_optionsViewOn) { OptionsWindow(allSceneObjects); }
    if (m_sceneViewOn) { SceneWindow(); }
    if (m_assetViewOn) { AssetWindow(); }
    if (Console::isOn) { Console::PerFrame(); }

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

void MasterUI::ObjectsWindow(std::vector<SceneObject*>& allSceneObjects)
{
    // SceneObjects Child
    ImGui::Begin("Objects", &m_objectsViewOn, m_windowFlagsChild);
    ImGui::Text("SceneObjects in Scene:");
    ImGui::Spacing();
    //m_sceneObjectIndex = 0;
    for (unsigned int i = 0; i < allSceneObjects.size(); i++)
    {
        const bool isSelected = (m_sceneObjectIndex == i);
        if (ImGui::Selectable(allSceneObjects[i]->GetName().c_str(), isSelected)) { m_selectedSceneObject = allSceneObjects[i]; m_sceneObjectIndex = i; }
        if (isSelected) { ImGui::SetItemDefaultFocus(); }

        if (allSceneObjects[i] == m_selectedSceneObject)
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
                        if (allSceneObjects[j] == m_selectedSceneObject)
                        {
                            indexToRemove = j;
                        }
                    }
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
            m_selectedSceneObject = allSceneObjects[allSceneObjects.size() - 1];
            m_sceneObjectIndex = allSceneObjects.size() - 1;
        }
    }
    if (ImGui::BeginPopup("NeedToRename"))
    {
        ImGui::Text("Rename your new SceneObject first!");
        ImGui::EndPopup();
    }
    ImGui::End();
}

void MasterUI::OptionsWindow(std::vector<SceneObject*>& allSceneObjects)
{
    // Options Child
    ImGui::Begin("Options", &m_optionsViewOn, m_windowFlagsChild);
    if (m_selectedSceneObject != nullptr)
    {
        m_selectedSceneObject->DrawHeaderUI();
        std::vector<Component*> components = m_selectedSceneObject->GetAllComponents();
        for (unsigned int i = 0; i < components.size(); i++) { components[i]->DrawUI(); }
        if (ImGui::Button("  +  ", ImVec2(ImGui::GetContentRegionAvail().x, 0))) { ImGui::OpenPopup("AddComponentPopup"); }
        ImGui::SameLine();
        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            ImVec2 size = ImVec2(100, 0);
            if (ImGui::Selectable("Cube", false, 0, size)) { m_selectedSceneObject->AddComponent(new Cube(nullptr, 1.0f, m_selectedSceneObject)); }
            if (ImGui::Selectable("Model", false, 0, size)) { m_selectedSceneObject->AddComponent(new Model(nullptr, m_selectedSceneObject, nullptr)); }
            if (ImGui::Selectable("Spinner", false, 0, size)) { m_selectedSceneObject->AddComponent(new Spinner(m_selectedSceneObject->GetModelPtr(), m_selectedSceneObject)); }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void MasterUI::SceneWindow()
{
    // Game View Child
    ImGui::Begin("Scene", &m_sceneViewOn, m_windowFlagsChild);
    m_sceneViewSize = ImGui::GetContentRegionAvail();
    ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_sceneFramebuffer->GetFramebuffer()), ImVec2(m_sceneViewSize.x, m_sceneViewSize.y), ImVec2(0, 1), ImVec2(1, 0), 0);
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
    ImGui::End();
}

void MasterUI::AssetWindow() 
{
    ImGui::Begin("Assets", &m_assetViewOn, m_windowFlagsChild);
    if (ImGui::TreeNode("Images"))
    {
        ImGui::Indent(20.0f);
        for (unsigned int i = 0; i < MasterTextures::textureList.size(); i++)
        {
            ImGui::Selectable(MasterTextures::textureList[i]->GetName().c_str());
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::Selectable("Remove", false, 0, ImVec2(100, 0))) { MasterTextures::RemoveTexture(MasterTextures::textureList[i]->GetName()); GreyfishParsing::SaveAssets(); }
                ImGui::EndPopup();
            }
            if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Right-click to open asset options"); }
        }
        ImGui::Unindent(20.0f);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Models"))
    {
        ImGui::Indent(20.0f);
        for (unsigned int i = 0; i < MasterObjMeshes::objMeshList.size(); i++)
        {
            ImGui::Selectable(MasterObjMeshes::objMeshList[i]->GetName().c_str());
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::Selectable("Remove", false, 0, ImVec2(100, 0))) { MasterObjMeshes::RemoveObjMesh(MasterObjMeshes::objMeshList[i]->GetName()); GreyfishParsing::SaveAssets(); }
                ImGui::EndPopup();
            }
            if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Right-click to open asset options"); }
        }
        ImGui::Unindent(20.0f);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Shaders"))
    {
        ImGui::Indent(20.0f);
        for (unsigned int i = 0; i < MasterShaders::shaderList.size(); i++)
        {
            ImGui::Selectable(MasterShaders::shaderList[i]->GetName().c_str());
            if (MasterShaders::shaderList.size() > 1)
            {
                if (MasterShaders::shaderList[i]->GetFilePath() != "Settings/EngineAssets/InEngineShaders/Lit")
                {
                    if (ImGui::BeginPopupContextItem())
                    {
                        if (ImGui::Selectable("Remove", false, 0, ImVec2(100, 0))) { MasterShaders::RemoveShader(MasterShaders::shaderList[i]->GetName()); GreyfishParsing::SaveAssets(); }
                        ImGui::EndPopup();
                    }
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Right-click to open asset options"); }
                }
            }
        }
        ImGui::Unindent(20.0f);
        ImGui::TreePop();
    }
    ImGui::End();
}

void MasterUI::Menu(std::vector<SceneObject*>& allSceneObjects)
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) 
            {
                allSceneObjects = std::vector<SceneObject*>();
                m_sceneObjectIndex = -1;
                m_selectedSceneObject = nullptr;
                m_openFile = "";
            }
            if (ImGui::MenuItem("Open")) 
            {
                nfdchar_t* outPath = NULL;
                nfdchar_t filters[] = "yaml";
                nfdchar_t defaultPath[] = "Assets\Scenes\\0";
                nfdresult_t result = NFD_OpenDialog(filters, defaultPath, &outPath);

                if (result == NFD_OKAY)
                {
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
                        bool success = GreyfishParsing::IsValidFile(filePath);
                        if (success) 
                        {
                            allSceneObjects = std::vector<SceneObject*>();
                            allSceneObjects = GreyfishParsing::LoadFileIntoSceneObjects(filePath);
                            m_sceneObjectIndex = -1;
                            m_selectedSceneObject = nullptr;
                            m_openFile = filePath;
                            AddRecentFile(filePath);
                        }
                        else 
                        {
                            Console::AddWarningMessage("Scene Dialog: Not a valid yaml file.");
                        }
                    }
                    else 
                    {
                        Console::AddWarningMessage("Scene Dialog: Wrong file extension.");
                    }
                }
                else if (result == NFD_ERROR)
                {
                    Console::AddErrorMessage("Scene Dialog: ", NFD_GetError());
                }
            }
            if (ImGui::BeginMenu("Open Recent")) 
            {
                for (unsigned int i = 0; i < 5; i++) 
                {
                    if (m_recentFiles[i] != "") 
                    {
                        if (ImGui::MenuItem(m_recentFiles[i].c_str())) 
                        {
                            if (GreyfishParsing::DoesFileExist(m_recentFiles[i])) 
                            {
                                allSceneObjects = std::vector<SceneObject*>();
                                allSceneObjects = GreyfishParsing::LoadFileIntoSceneObjects(m_recentFiles[i]);
                                m_sceneObjectIndex = -1;
                                m_selectedSceneObject = nullptr;
                                AddRecentFile(m_recentFiles[i]);
                            }
                            else 
                            {
                                m_recentFiles.erase(m_recentFiles.begin() + i);
                                m_recentFiles.push_back("");
                                SaveRecentFileList();
                                Console::AddWarningMessage("MasterUI: Removing recent file entry.");
                            }
                        }
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save")) 
            {
                if (m_openFile != "") 
                {
                    GreyfishParsing::SaveSceneObjectsIntoFile(m_openFile, allSceneObjects);
                }
                else 
                {
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
            ImGui::MenuItem("Objects", NULL, &m_objectsViewOn);
            ImGui::MenuItem("Options", NULL, &m_optionsViewOn);
            ImGui::MenuItem("Game", NULL, &m_sceneViewOn);
            ImGui::MenuItem("Console", NULL, &Console::isOn);
            ImGui::MenuItem("Assets", NULL, &m_assetViewOn);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Add"))
        {
            if (ImGui::MenuItem("Image")) 
            {
                nfdchar_t filters[] = "png,jpg,jpeg";
                nfdchar_t defaultPath[] = "Assets\Images\\0";
                LoadAsset(defaultPath, filters, "Image");
            }
            if (ImGui::MenuItem("Model")) 
            {
                nfdchar_t defaultPath[] = "Assets\Models\\0";
                nfdchar_t filters[] = "obj";
                LoadAsset(defaultPath, filters, "Model");
            }
            if (ImGui::MenuItem("Shader")) 
            {
                nfdchar_t defaultPath[] = "Assets\Shaders\\0";
                nfdchar_t filters[] = "vert";
                LoadAsset(defaultPath, filters, "Shader");
            }
            ImGui::EndMenu();
        }
        ImGui::Spacing();ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();ImGui::Spacing();
        ImGui::Text("%.1f", m_io->Framerate);
        ImGui::EndMenuBar();
    }
}

void MasterUI::Save(std::vector<SceneObject*>& allSceneObjects)
{
    nfdchar_t* outPath = NULL;
    nfdchar_t filters[] = "yaml";
    nfdchar_t defaultPath[] = "Assets\Scenes\\0";
    nfdresult_t result = NFD_SaveDialog(filters, defaultPath, &outPath);

    if (result == NFD_OKAY)
    {
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
        GreyfishParsing::SaveSceneObjectsIntoFile(filePath, allSceneObjects);
        m_openFile = filePath;
        AddRecentFile(filePath);   
    }
    else if (result == NFD_ERROR) 
    {
        Console::AddErrorMessage("Scene Dialog: ", NFD_GetError());
    }
}

void MasterUI::AddRecentFile(std::string recentFile)
{
    bool alreadyExists = false;
    int index = -1;
    for (unsigned int i = 0; i < m_recentFiles.size(); i++) 
    {
        if (recentFile == m_recentFiles[i]) { alreadyExists = true; index = i; break; }
    }

    if (alreadyExists) 
    {
        m_recentFiles.erase(m_recentFiles.begin() + index);
    }
    std::vector<std::string> temp = std::vector<std::string>(5);
    temp[0] = recentFile;
    temp[1] = m_recentFiles[0];
    temp[2] = m_recentFiles[1];
    temp[3] = m_recentFiles[2];
    temp[4] = m_recentFiles[3];
    m_recentFiles = temp;

    SaveRecentFileList();
}

void MasterUI::SaveRecentFileList() 
{
    std::ofstream newSceneFile{ "Settings/RecentFiles.yaml" };
    YAML::Emitter out;

    out << YAML::BeginSeq;
    for (unsigned int i = 0; i < 5; i++)
    {
        out << m_recentFiles[i];
    }
    out << YAML::EndSeq;

    newSceneFile << out.c_str();
    newSceneFile.close();
}

void MasterUI::LoadRecentFiles()
{
    YAML::Node config = YAML::LoadFile("Settings/RecentFiles.yaml");

    for (unsigned int i = 0; i < 5; i++) 
    {
        if (config[i].as<std::string>() != "")
        {
            m_recentFiles[i] = config[i].as<std::string>();
        }
    }
}

void MasterUI::LoadAsset(nfdchar_t* defaultPath, nfdchar_t* filters, std::string type) 
{
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_OpenDialog(filters, defaultPath, &outPath);

    if (result == NFD_OKAY)
    {
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
        if ((type == "Image") && ((ex == "png") || (ex == "jpg") || (ex == "jpeg")))
        {
            Console::AddMessage("Asset Dialog: Image Success!");
            MasterTextures::NewTexture(filePath);
            GreyfishParsing::SaveAssets();
        }
        else if ((type == "Model") && (ex == "obj"))
        {
            Console::AddMessage("Asset Dialog: Model Success!");
            MasterObjMeshes::NewObjMesh(filePath);
            GreyfishParsing::SaveAssets();
        }
        else if ((type == "Shader") && (ex == "vert"))
        {
            Console::AddMessage("Asset Dialog: Shader Success!");
            std::string filePathProper = "";
            for (unsigned int i = 0; i < filePath.size(); i++)
            {
                if (filePath[i] == '.') 
                {
                    break;
                }
                filePathProper.push_back(filePath[i]);
            }
            MasterShaders::NewShader(filePathProper);
            GreyfishParsing::SaveAssets();
        }
        else
        {
            Console::AddWarningMessage("Scene Dialog: Wrong file extension.");
        }
    }
    else if (result == NFD_ERROR)
    {
        Console::AddErrorMessage("Asset Dialog: ", NFD_GetError());
    }
}

void MasterUI::CleanUp() 
{
    GreyfishParsing::SaveAssets();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

GLFWwindow* MasterUI::GetWindow() { return m_window; }
SceneObject* MasterUI::GetSelectedSceneObject() { return m_selectedSceneObject; }
ImVec2 MasterUI::GetSize() { return m_size; }
ImVec2 MasterUI::GetOffset() { return m_offset; }
float MasterUI::GetMouseWheel() { return m_mouseWheel; }
Motion MasterUI::GetCamMotion() { return m_camMotion; }
ImVec2 MasterUI::GetSceneViewSize() { return m_sceneViewSize; }
Framebuffer* MasterUI::GetSceneFramebuffer() { return m_sceneFramebuffer; }

void MasterUI::SetWindow(GLFWwindow* window) { m_window = window; }
void MasterUI::SetSelectedSceneObject(SceneObject* selectedSceneObject) { m_selectedSceneObject = selectedSceneObject; }
void MasterUI::SetSize(ImVec2 size) { m_size = size; }
void MasterUI::SetOffset(ImVec2 offset) { m_offset = offset; }
void MasterUI::SetMouseWheel(float mouseWheel) { m_mouseWheel = mouseWheel; }
void MasterUI::SetCamMotion(Motion motion) { m_camMotion = motion; }
void MasterUI::SetSceneViewSize(ImVec2 gameViewSize) { m_sceneViewSize = gameViewSize; }
void MasterUI::SetSceneFramebuffer(Framebuffer* sceneFramebuffer) { m_sceneFramebuffer = sceneFramebuffer; }
