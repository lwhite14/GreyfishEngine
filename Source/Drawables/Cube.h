#pragma once

#include "TriangleMesh.h"
#include "../SceneObjects/Component.h"
#include "../Texture.h"
#include "../GLSLProgram.h"

class Cube : public TriangleMesh, public Component
{
private:
	Texture* m_texture;
	GLSLProgram *m_prog;

	char m_textureName[64];

public:
	Cube(GLSLProgram* prog, Texture* texture, GLfloat size);
	void Update();
	void Render();
	void DrawUI();
};