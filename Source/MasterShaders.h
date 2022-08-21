#ifndef MASTERSHADERS_H
#define MASTERSHADERS_H

#include <map>
#include <string>
#include <vector>
#include "GLSLProgram.h"

class MasterShaders
{
public:
	inline static std::vector<GLSLProgram*> shaderList;

    inline static void NewShader(std::string shaderName)
	{
		GLSLProgram* prog = new GLSLProgram();

        std::string start = "Source/Shaders/";
        std::string vertEnd = ".vert";
        std::string fragEnd = ".frag";

        std::string vert = start + shaderName + vertEnd;
        std::string frag = start + shaderName + fragEnd;

        try
        {
            prog->CompileShader(vert.c_str());
            prog->CompileShader(frag.c_str());
            prog->Link();
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