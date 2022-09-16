#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "UserInterface/Console.h"

class Framebuffer
{
private:
    unsigned int m_framebuffer;
    unsigned int m_textureColorbuffer;

    int m_gameViewWidth;
    int m_gameViewHeight;

public:
    Framebuffer() {  }

    Framebuffer(int gameViewWidth, int gameViewHeight)
    {
        m_gameViewWidth = gameViewWidth;
        m_gameViewHeight = gameViewHeight;

        // framebuffer configuration
        // -------------------------
        //unsigned int framebuffer;
        glGenFramebuffers(1, &m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        // create a color attachment texture
        //unsigned int textureColorbuffer;
        glGenTextures(1, &m_textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gameViewWidth, gameViewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer, 0);
        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, gameViewWidth, gameViewHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Console::AddMessage("Framebuffer is not complete!");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    unsigned int GetFramebuffer() 
    {
        return m_framebuffer;
    }

    int GetGameViewWidth() { return m_gameViewWidth; }
    int GetGameViewHeight() { return m_gameViewHeight; }
};

#endif //FRAMEBUFFER_H