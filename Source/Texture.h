#pragma once

#include <glad/glad.h>
#include <iostream>

class Texture 
{
private:
	GLuint m_texID;

	void LoadTexture(const char* filePath);

public:
	Texture(const char* filePath);
	unsigned char* LoadPixels(const char*, int& width, int& height, bool flip = true);
	GLuint GetTexture();
	void Bind();

};

