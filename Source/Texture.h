#pragma once

#include <glad/glad.h>
#include <iostream>

class Texture 
{
public:
	static GLuint LoadTexture(const std::string& filePath);
	static GLuint* LoadTexturePtr(const std::string& filePath);
	static unsigned char* LoadPixels(const std::string& filePath, int& width, int& height, bool flip = true);
};

