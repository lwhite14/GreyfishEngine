#ifndef MASTERSHADERS_H
#define MASTERSHADERS_H

#include <map>
#include <string>
#include <vector>
#include "GLSLProgram.h"
#include "UserInterface/Console.h"

class MasterShaders
{
public:
	inline static std::vector<GLSLProgram*> shaderList;

    inline static void NewShader(std::string shaderName, bool canRemove = true)
	{
        std::string vertEnd = ".vert";
        std::string fragEnd = ".frag";

        std::string vert = shaderName + vertEnd;
        std::string frag = shaderName + fragEnd;

        bool isName = false;
        std::string name = "";
        for (unsigned int i = 0; i < shaderName.size(); i++)
        {
            if (shaderName[i] == '.')
            {
                isName = true;
            }
            if (!isName)
            {
                name.push_back(shaderName[i]);
                if ((shaderName[i] == '/') || (shaderName[i] == '\\'))
                {
                    name = "";
                }
            }
        }

        FILE* fileVert = fopen(vert.c_str(), "r");
        FILE* fileFrag = fopen(frag.c_str(), "r");

        if (fileVert && fileFrag)
        {
            fclose(fileVert);
            fclose(fileFrag);
        }
        else
        {
            Console::AddWarningMessage("Shader: '" + name + "' location is not valid, file has either been moved or deleted.");
            return;
        }

        for (unsigned int i = 0; i < shaderList.size(); i++)
        {
            if (shaderList[i]->GetName() == name) 
            {
                Console::AddMessage("MasterShaders: Shader already added.");
                return;
            }
        }

        GLSLProgram* prog = new GLSLProgram();
        try
        {
            prog->CompileShader(vert.c_str());
            prog->CompileShader(frag.c_str());
            prog->Link();
            prog->SetName(name);
            prog->SetCanRemove(canRemove);
            prog->SetFilePath(shaderName);
            shaderList.push_back(prog);
        }
        catch (GLSLProgramException& e)
        {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
	}

    inline static void RemoveShader(std::string fileName)
    {
        if (shaderList.size() == 1) 
        {
            Console::AddWarningMessage("There must always be at least one shader!");
            return;
        }
        for (unsigned int i = 0; i < shaderList.size(); i++)
        {
            if (shaderList[i]->GetName() == fileName)
            {
                shaderList.erase(shaderList.begin() + i);
            }
        }
    }
};

#endif //MASTERSHADERS_H