#ifndef SYSTEMINFORMATION_H
#define SYSTEMINFORMATION_H

#include <glad/glad.h>
#include <iostream>
#include "UserInterface/Console.h"

namespace SystemInformation
{
    void OutputInformation();
    std::string CreateString(const GLubyte* input);

    void OutputInformation()
    {
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* version = glGetString(GL_VERSION);
        const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        GLint major, minor, samples, sampleBuffers;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        glGetIntegerv(GL_SAMPLES, &samples);
        glGetIntegerv(GL_SAMPLE_BUFFERS, &sampleBuffers);

        Console::AddMessage("GL Vendor: " + CreateString(vendor));
        Console::AddMessage("GL Renderer: " + CreateString(renderer));
        Console::AddMessage("GL Version: " + CreateString(version));
        Console::AddMessage("GL Version: " + std::to_string(major) + "." + std::to_string(minor));
        Console::AddMessage("GLSL Version: " + CreateString(glslVersion));
        Console::AddMessage("MSAA samples: " + std::to_string(samples));
        Console::AddMessage("MSAA buffers: " + std::to_string(sampleBuffers));
    }

    std::string CreateString(const GLubyte* input)
    {
        std::string output = "";
        for (char c = *input; c; c = *++input)
        {
            output.push_back(c);
        }
        return output;
    }
}

#endif //SYSTEMINFORMATION_H
