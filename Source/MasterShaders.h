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

    inline static void NewShader(std::string shaderName)
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
            prog->SetFilePath(shaderName);
            shaderList.push_back(prog);
        }
        catch (GLSLProgramException& e)
        {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
	}

};

#endif //MASTERSHADERS_H