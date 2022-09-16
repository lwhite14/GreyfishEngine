#pragma once

#include <glad/glad.h>
#include <iostream>

class Texture 
{
private:
	std::string m_name;
	GLuint m_texID;

	void LoadTexture(const char* filePath);

public:
	Texture();
	Texture(const char* filePath);
	unsigned char* LoadPixels(const char*, int& width, int& height, bool flip = true);
	void Bind();

	GLuint GetTexture();
	std::string GetName();

	void SetName(std::string name);

};

