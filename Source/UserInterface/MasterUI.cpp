#include "MasterUI.h"

#include <yaml-cpp/yaml.h>
#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/imgui_impl_glfw.h"
#include "../Dependencies/imgui/imgui_impl_opengl3.h"
#include "../Drawables/Cube.h"
#include "../Drawables/ObjMesh.h"
#include "../SceneObjects/Spinner.h"
#include "../MasterShaders.h"
#include "../MasterTextures.h"

MasterUI::MasterUI() { }

MasterUI::MasterUI(GLFWwindow* window, ImVec2 size) :
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
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    m_io = &ImGui::GetIO();
    m_io->Fonts->AddFontFromFileTTF("Media/Fonts/Lato-Black.ttf", 12);

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowPadding = ImVec2(4.0f, 4.0f);
    style.FramePadding = ImVec2(4.0f, 4.0f);
    style.CellPadding = ImVec2(4.0f, 4.0f);
    style.ItemSpacing = ImVec2(4.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
    style.IndentSpacing = 10.0f;
    style.ScrollbarSize = 10.0f;
    style.GrabMinSize = 10.0f;

    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;

    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.LogSliderDeadzone = 0.0f;
    style.TabRounding = 0.0f;

    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
    style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);

    //ImVec4* colors = ImGui::GetStyle().Colors;
    //colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    //colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    //colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    //colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    //colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    //colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    //colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    //colors[ImGuiCol_FrameBg] = ImVec4(0.67f, 0.67f, 0.67f, 0.35f);
    //colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 0.35f);
    //colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 0.35f);
    //colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    //colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    //colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    //colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    //colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    //colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    //colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    //colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.99f, 1.00f, 1.00f);
    //colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    //colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    //colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    //colors[ImGuiCol_Button] = ImVec4(0.67f, 0.67f, 0.67f, 0.35f);
    //colors[ImGuiCol_ButtonHovered] = ImVec4(0.16f, 0.16f, 0.16f, 0.35f);
    //colors[ImGuiCol_ButtonActive] = ImVec4(0.16f, 0.16f, 0.16f, 0.35f);
    //colors[ImGuiCol_Header] = ImVec4(0.67f, 0.67f, 0.67f, 0.35f);
    //colors[ImGuiCol_HeaderHovered] = ImVec4(0.16f, 0.16f, 0.16f, 0.35f);
    //colors[ImGuiCol_HeaderActive] = ImVec4(0.16f, 0.16f, 0.16f, 0.35f);
    //colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    //colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    //colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    //colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    //colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    //colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    //colors[ImGuiCol_Tab] = ImVec4(0.67f, 0.67f, 0.67f, 0.35f);
    //colors[ImGuiCol_TabHovered] = ImVec4(0.16f, 0.16f, 0.16f, 0.35f);
    //colors[ImGuiCol_TabActive] = ImVec4(0.16f, 0.16f, 0.16f, 0.35f);
    //colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    //colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    //colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    //colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    //colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    //colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    //colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    //colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    //colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    //colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    //colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    //colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    //colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    //colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    //colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    //colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    //colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    //colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    //colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    m_windowFlags |= ImGuiWindowFlags_NoResize;
    m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
    m_windowFlags |= ImGuiWindowFlags_NoScrollbar;
    m_windowFlags |= ImGuiWindowFlags_MenuBar;
    m_windowFlags |= ImGuiWindowFlags_NoMove;
    m_windowFlags |= ImGuiWindowFlags_NoCollapse;
    //m_windowFlags |= ImGuiWindowFlags_NoNav;
    //m_windowFlags |= ImGuiWindowFlags_NoBackground;
    //m_windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
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

    //FramerateWindow();

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
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 window_pos, window_pos_pivot;
    window_pos_pivot.x = (0 & 1) ? 1.0f : 0.0f;
    window_pos_pivot.y = (0 & 2) ? 1.0f : 0.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowSize(ImVec2(85, 20));
    ImGui::SetNextWindowViewport(viewport->ID);
    window_flags |= ImGuiWindowFlags_NoMove;
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
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open")) 
            {
                allSceneObjects = std::vector<SceneObject*>();

                std::string path = "Media/Scenes/";
                std::string name = "ExampleScene";
                std::string extension = ".yaml";

                YAML::Node config = YAML::LoadFile(path + name + extension);

                for (unsigned int i = 0; i < config["SceneObjects"].size(); i++) 
                {
                    YAML::Node node = config["SceneObjects"][i];
                    YAML::Node dimensions = node["dimensions"];
                    std::string progName = node["glslProgramName"].as<std::string>();
                    glm::vec3 position = glm::vec3(dimensions["posX"].as<float>(), dimensions["posY"].as<float>(), dimensions["posZ"].as<float>());
                    glm::vec3 rotation = glm::vec3(dimensions["rotX"].as<float>(), dimensions["rotY"].as<float>(), dimensions["rotZ"].as<float>());
                    glm::vec3 scale = glm::vec3(dimensions["scaX"].as<float>(), dimensions["scaY"].as<float>(), dimensions["scaZ"].as<float>());
                    GLSLProgram* prog = new GLSLProgram;
                    for (unsigned int j = 0; j < MasterShaders::shaderList.size(); j++) 
                    {
                        if (progName == MasterShaders::shaderList[j]->GetName()) 
                        {
                            prog = MasterShaders::shaderList[j];
                        }
                    }
                    allSceneObjects.push_back(new SceneObject(node["name"].as<std::string>(), position, rotation, scale, prog));

                    YAML::Node components = node["components"];
                    for (unsigned int j = 0; j < components.size(); j++) 
                    {
                        if (components[j]["cube"]) 
                        {
                            std::string texName = components[j]["cube"]["texture"].as<std::string>();
                            Texture* texture = new Texture;
                            for (unsigned int y = 0; y < MasterTextures::textureList.size(); y++) 
                            {
                                if (texName == MasterTextures::textureList[y]->GetName()) 
                                {
                                    texture = MasterTextures::textureList[y];
                                }
                            }
                            glm::vec3 matAmbient = glm::vec3(components[j]["cube"]["matAmbientX"].as<float>(), components[j]["cube"]["matAmbientY"].as<float>(), components[j]["cube"]["matAmbientZ"].as<float>());
                            glm::vec3 matDiffuse = glm::vec3(components[j]["cube"]["matDiffuseX"].as<float>(), components[j]["cube"]["matDiffuseY"].as<float>(), components[j]["cube"]["matDiffuseZ"].as<float>());
                            glm::vec3 matSpecular = glm::vec3(components[j]["cube"]["matSpecularX"].as<float>(), components[j]["cube"]["matSpecularY"].as<float>(), components[j]["cube"]["matSpecularZ"].as<float>());
                            float matShininess = components[j]["cube"]["matShininess"].as<float>();
                            Cube* cube = new Cube(texture, 1.0f, allSceneObjects[i]);
                            cube->SetMatAmbient(matAmbient);
                            cube->SetMatDiffuse(matDiffuse);
                            cube->SetMatSpecular(matSpecular);
                            cube->SetMatShininess(matShininess);
                            allSceneObjects[i]->AddComponent(cube);
                        }
                        if (components[j]["objMesh"])
                        {
                            std::string texName = components[j]["objMesh"]["texture"].as<std::string>();
                            Texture* texture = new Texture;
                            for (unsigned int y = 0; y < MasterTextures::textureList.size(); y++)
                            {
                                if (texName == MasterTextures::textureList[y]->GetName())
                                {
                                    texture = MasterTextures::textureList[y];
                                }
                            }
                            glm::vec3 matAmbient = glm::vec3(components[j]["objMesh"]["matAmbientX"].as<float>(), components[j]["objMesh"]["matAmbientY"].as<float>(), components[j]["objMesh"]["matAmbientZ"].as<float>());
                            glm::vec3 matDiffuse = glm::vec3(components[j]["objMesh"]["matDiffuseX"].as<float>(), components[j]["objMesh"]["matDiffuseY"].as<float>(), components[j]["objMesh"]["matDiffuseZ"].as<float>());
                            glm::vec3 matSpecular = glm::vec3(components[j]["objMesh"]["matSpecularX"].as<float>(), components[j]["objMesh"]["matSpecularY"].as<float>(), components[j]["objMesh"]["matSpecularZ"].as<float>());
                            float matShininess = components[j]["objMesh"]["matShininess"].as<float>();
                            ObjMesh* objMesh = ObjMesh::Load("Media/Models/suzanne.obj", allSceneObjects[i], texture);
                            objMesh->SetMatAmbient(matAmbient);
                            objMesh->SetMatDiffuse(matDiffuse);
                            objMesh->SetMatSpecular(matSpecular);
                            objMesh->SetMatShininess(matShininess);
                            allSceneObjects[i]->AddComponent(objMesh);
                        }
                        if (components[j]["spinner"])
                        { 
                            Spinner* spinner = new Spinner(allSceneObjects[i]->GetModelPtr(), allSceneObjects[i]);
                            spinner->SetSpeed(components[j]["spinner"]["speed"].as<float>());
                            allSceneObjects[i]->AddComponent(spinner);
                        }
                    }
                }
                m_sceneObjectIndex = -1;
                selectedSceneObject = nullptr;
                m_selectedSceneObject = nullptr;
            }
            if (ImGui::MenuItem("Open Recent")) {}
            if (ImGui::MenuItem("Save")) 
            {
                std::string path = "Media/Scenes/";
                std::string name = "ExampleScene";
                std::string extension = ".yaml";

                std::ofstream newSceneFile{ path + name + extension };

                YAML::Emitter out;
                out << YAML::BeginMap; // SceneObjects Map

                out << YAML::Key << "SceneObjects" << YAML::Value;
                out << YAML::BeginSeq;
                for (unsigned int i = 0; i < allSceneObjects.size(); i++)
                {
                    out << YAML::BeginMap; // Object Instance Map

                    out << YAML::Key << "name" << YAML::Value << allSceneObjects[i]->GetName();
                    out << YAML::Key << "glslProgramName" << YAML::Value << allSceneObjects[i]->GetGLSLProgram()->GetName();

                    allSceneObjects[i]->GetDimensions()->Serialization(out);

                    out << YAML::Key << "components" << YAML::Value;
                    out << YAML::BeginSeq;

                    for (unsigned int j = 0; j < allSceneObjects[i]->GetComponents().size(); j++)
                    {
                        allSceneObjects[i]->GetComponents()[j]->Serialization(out);
                    }

                    out << YAML::EndSeq;
                    out << YAML::EndMap; // Object Instance Map 
                }
                out << YAML::EndSeq;
                out << YAML::EndMap; // SceneObjects Map

                newSceneFile << out.c_str();
                newSceneFile.close();
            }
            if (ImGui::MenuItem("Save As")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
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