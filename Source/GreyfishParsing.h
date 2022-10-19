#ifndef GREYFISHPARSING_H
#define GREYFISHPARSING_H

#include <string>
#include <vector>
#include <fstream>
#include "SceneObjects/SceneObject.h"
#include "Drawables/Model.h"
#include "Drawables/ObjMesh.h"
#include "Drawables/Cube.h"
#include "SceneObjects/Component.h"
#include "SceneObjects/Spinner.h"
#include "MasterObjMeshes.h"
#include "MasterShaders.h"
#include "MasterTextures.h"

namespace GreyfishParsing
{
    static inline bool IsValidFile(std::string filePath) 
    {
        YAML::Node config = YAML::LoadFile(filePath);
        if (config["Program"])
        {
            if (config["Program"].as<std::string>() == "GreyfishEngine")
            {
                return true;
            }
        }
        return false;
    }

    static inline bool DoesFileExist(std::string filePath)
    {
        YAML::Node config;
        try
        {
            YAML::Node config = YAML::LoadFile(filePath);
            return true;
        }
        catch (YAML::Exception ex)
        {
            Console::AddErrorMessage("GreyfishParsing: " + ex.msg);
            return false;
        }
    }

	static inline std::vector<SceneObject*> LoadFileIntoSceneObjects(std::string filePath)
	{
        std::vector<SceneObject*> allSceneObjects = std::vector<SceneObject*>();

        YAML::Node config = YAML::LoadFile(filePath);

        for (unsigned int i = 0; i < config["SceneObjects"].size(); i++)
        {
            YAML::Node node = config["SceneObjects"][i];
            YAML::Node dimensions = node["dimensions"];
            std::string progName = node["glslProgramName"].as<std::string>();
            glm::vec3 position = glm::vec3(dimensions["posX"].as<float>(), dimensions["posY"].as<float>(), dimensions["posZ"].as<float>());
            glm::vec3 rotation = glm::vec3(dimensions["rotX"].as<float>(), dimensions["rotY"].as<float>(), dimensions["rotZ"].as<float>());
            glm::vec3 scale = glm::vec3(dimensions["scaX"].as<float>(), dimensions["scaY"].as<float>(), dimensions["scaZ"].as<float>());
            GLSLProgram* prog = new GLSLProgram;
            bool doesExist = false;
            for (unsigned int j = 0; j < MasterShaders::shaderList.size(); j++)
            {
                if (progName == MasterShaders::shaderList[j]->GetName())
                {
                    doesExist = true;
                    prog = MasterShaders::shaderList[j];
                }
            }
            if (!doesExist) 
            {
                prog = MasterShaders::shaderList[0];
                Console::AddWarningMessage("Shader: '" + progName + "' on SceneObject: '" + node["name"].as<std::string>() + "' in the scene file does not exist within project, the object shader has switched to an another shader.");
            }
            allSceneObjects.push_back(new SceneObject(node["name"].as<std::string>(), position, rotation, scale, prog));

            YAML::Node components = node["components"];
            for (unsigned int j = 0; j < components.size(); j++)
            {
                if (components[j]["cube"])
                {
                    std::string texName = components[j]["cube"]["texture"].as<std::string>();
                    Texture* texture = nullptr;
                    doesExist = false;
                    for (unsigned int y = 0; y < MasterTextures::textureList.size(); y++)
                    {
                        if (texName == MasterTextures::textureList[y]->GetName())
                        {
                            texture = MasterTextures::textureList[y];
                            doesExist = true;
                        }
                    }
                    if (!doesExist) 
                    {
                        Console::AddWarningMessage("Texture: '" + texName + "' on SceneObject: '" + node["name"].as<std::string>() + "' in the scene file does not exist within the project, make sure to add a texture to this drawable before saving.");
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
                if (components[j]["model"])
                {
                    std::string texName = components[j]["model"]["texture"].as<std::string>();
                    Texture* texture = nullptr;
                    doesExist = false;
                    for (unsigned int y = 0; y < MasterTextures::textureList.size(); y++)
                    {
                        if (texName == MasterTextures::textureList[y]->GetName())
                        {
                            texture = MasterTextures::textureList[y];
                            doesExist = true;
                        }
                    }
                    if (!doesExist) 
                    {
                        Console::AddWarningMessage("Texture: '" + texName + "' on SceneObject: '" + node["name"].as<std::string>() + "' in the scene file does not exist within the project, make sure to add a texture to this drawable before saving.");
                    }
                    glm::vec3 matAmbient = glm::vec3(components[j]["model"]["matAmbientX"].as<float>(), components[j]["model"]["matAmbientY"].as<float>(), components[j]["model"]["matAmbientZ"].as<float>());
                    glm::vec3 matDiffuse = glm::vec3(components[j]["model"]["matDiffuseX"].as<float>(), components[j]["model"]["matDiffuseY"].as<float>(), components[j]["model"]["matDiffuseZ"].as<float>());
                    glm::vec3 matSpecular = glm::vec3(components[j]["model"]["matSpecularX"].as<float>(), components[j]["model"]["matSpecularY"].as<float>(), components[j]["model"]["matSpecularZ"].as<float>());
                    float matShininess = components[j]["model"]["matShininess"].as<float>();
                    std::string objMeshName = components[j]["model"]["objMesh"].as<std::string>();
                    ObjMesh* objMesh = nullptr;
                    doesExist = false;
                    for (unsigned int y = 0; y < MasterObjMeshes::objMeshList.size(); y++) 
                    {
                        if (objMeshName == MasterObjMeshes::objMeshList[y]->GetName())
                        {
                            objMesh = MasterObjMeshes::objMeshList[y];
                            doesExist = true;
                        }
                    }
                    if (!doesExist) 
                    {
                        Console::AddWarningMessage("ObjMesh: '" + objMeshName + "' on SceneObject: '" + node["name"].as<std::string>() + "' in the scene file does not exist within the project, make sure to add a texture to this drawable before saving.");
                    }
                    Model* model = new Model(objMesh, allSceneObjects[i], texture);
                    model->SetMatAmbient(matAmbient);
                    model->SetMatDiffuse(matDiffuse);
                    model->SetMatSpecular(matSpecular);
                    model->SetMatShininess(matShininess);
                    allSceneObjects[i]->AddComponent(model);
                }
                if (components[j]["spinner"])
                {
                    Spinner* spinner = new Spinner(allSceneObjects[i]->GetModelPtr(), allSceneObjects[i]);
                    spinner->SetSpeed(components[j]["spinner"]["speed"].as<float>());
                    allSceneObjects[i]->AddComponent(spinner);
                }
            }
        }
        return allSceneObjects;
	}

    static inline void SaveSceneObjectsIntoFile(std::string filePath, const std::vector<SceneObject*>& sceneObjects)
    {
        std::ofstream newSceneFile{ filePath };

        YAML::Emitter out;

        out << YAML::BeginMap;
        out << YAML::Key << "Program" << YAML::Value << "GreyfishEngine";      

        out << YAML::Key << "SceneObjects" << YAML::Value;
        out << YAML::BeginSeq;
        for (unsigned int i = 0; i < sceneObjects.size(); i++)
        {
            out << YAML::BeginMap; // Object Instance Map

            out << YAML::Key << "name" << YAML::Value << sceneObjects[i]->GetName();
            out << YAML::Key << "glslProgramName" << YAML::Value << sceneObjects[i]->GetGLSLProgram()->GetName();

            sceneObjects[i]->GetDimensions()->Serialization(out);

            out << YAML::Key << "components" << YAML::Value;
            out << YAML::BeginSeq;

            for (unsigned int j = 0; j < sceneObjects[i]->GetComponents().size(); j++)
            {
                sceneObjects[i]->GetComponents()[j]->Serialization(out);
            }

            out << YAML::EndSeq;
            out << YAML::EndMap; // Object Instance Map 
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;

        newSceneFile << out.c_str();
        newSceneFile.close();
    }

    static inline void SaveAssets() 
    {
        std::ofstream newSceneFile{ "Settings/Assets.yaml" };
        YAML::Emitter out;

        out << YAML::BeginMap;
        out << YAML::Key << "images";
        out << YAML::Value << YAML::BeginSeq;
        for (unsigned int i = 0; i < MasterTextures::textureList.size(); i++)
        {
            out << MasterTextures::textureList[i]->GetFilePath();
        }
        out << YAML::EndSeq;

        out << YAML::Key << "objMeshes";
        out << YAML::Value << YAML::BeginSeq;
        for (unsigned int i = 0; i < MasterObjMeshes::objMeshList.size(); i++)
        {
            out << MasterObjMeshes::objMeshList[i]->GetFilePath();
        }
        out << YAML::EndSeq;

        out << YAML::Key << "shaders";
        out << YAML::Value << YAML::BeginSeq;
        for (unsigned int i = 0; i < MasterShaders::shaderList.size(); i++)
        {
            if (MasterShaders::shaderList[i]->GetFilePath() != "Settings/EngineAssets/InEngineShaders/Lit")
            {
                out << MasterShaders::shaderList[i]->GetFilePath();
            }
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;

        newSceneFile << out.c_str();
        newSceneFile.close();
    }

    static inline void LoadAssets() 
    {
        YAML::Node config = YAML::LoadFile("Settings/Assets.yaml");


        MasterShaders::NewShader("Settings/EngineAssets/InEngineShaders/Lit", false);
        for (unsigned int i = 0; i < config["images"].size(); i++) 
        {
            MasterTextures::NewTexture(config["images"][i].as<std::string>());
        }
        for (unsigned int i = 0; i < config["objMeshes"].size(); i++) 
        {
            MasterObjMeshes::NewObjMesh(config["objMeshes"][i].as<std::string>());
        }
        for (unsigned int i = 0; i < config["shaders"].size(); i++) 
        {
            MasterShaders::NewShader(config["shaders"][i].as<std::string>());
        }
    }
}

#endif //GREYFISHPARSING_H