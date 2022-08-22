#include "Texture.h"
#include "Dependencies/stb/stb_image.h"

Texture::Texture(const char* filePath)
{
    LoadTexture(filePath);
    m_name = "";
    std::string name = filePath;
    for (unsigned int i = 0; i < name.size(); i++) 
    {
        if (!(i < 13)) 
        {
            m_name.push_back(name[i]);
        }
    }
}

void Texture::LoadTexture(const char* filePath) 
{
    int width, height;
    unsigned char* data = Texture::LoadPixels(filePath, width, height);
    GLuint tex = 0;
    if (data != nullptr) 
    {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }

    m_texID = tex;
}

unsigned char* Texture::LoadPixels(const char* filePath, int& width, int& height, bool flip)
{
    int bytesPerPix;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(filePath, &width, &height, &bytesPerPix, 4);
    return data;
}

void Texture::Bind() 
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texID);
}

GLuint Texture::GetTexture() { return m_texID; }
std::string Texture::GetName() { return m_name; }