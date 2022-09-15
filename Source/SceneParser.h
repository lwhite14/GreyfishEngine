#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <string>
#include <vector>
#include "SceneObjects/SceneObject.h"

namespace SceneParser
{
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
        return allSceneObjects;
	}

    static inline void SaveSceneObjectsIntoFile(std::string filePath, const std::vector<SceneObject*>& sceneObjects)
    {
        std::ofstream newSceneFile{ filePath };

        YAML::Emitter out;
        out << YAML::BeginMap; // SceneObjects Map

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
        out << YAML::EndMap; // SceneObjects Map

        newSceneFile << out.c_str();
        newSceneFile.close();
    }
}

#endif //SCENEPARSER_H