#include "Texture.h"
#include "stb/stb_image.h"

GLuint Texture::LoadTexture(const std::string& filePath) 
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

    return tex;
}

GLuint* Texture::LoadTexturePtr(const std::string& filePath) 
{
    int width, height;
    unsigned char* data = Texture::LoadPixels(filePath, width, height);
    GLuint* tex = new GLuint;
    if (data != nullptr)
    {
        glGenTextures(1, tex);
        glBindTexture(GL_TEXTURE_2D, *tex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }

    return tex;
}

unsigned char* Texture::LoadPixels(const std::string& filePath, int& width, int& height, bool flip)
{
    int bytesPerPix;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &bytesPerPix, 4);
    return data;
}