#ifndef SYSTEMINFORMATION_H
#define SYSTEMINFORMATION_H

#include <glad/glad.h>
#include <iostream>

namespace SystemInformation
{
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

        printf("-------------------------------------------------------------\n");
        printf("GL Vendor    : %s\n", vendor);
        printf("GL Renderer  : %s\n", renderer);
        printf("GL Version   : %s\n", version);
        printf("GL Version   : %d.%d\n", major, minor);
        printf("GLSL Version : %s\n", glslVersion);
        printf("MSAA samples : %d\n", samples);
        printf("MSAA buffers : %d\n", sampleBuffers);
        printf("-------------------------------------------------------------\n");
    }
}

#endif //SYSTEMINFORMATION_H
