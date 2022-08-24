#pragma once

#include "TriangleMesh.h"
#include "../SceneObjects/Component.h"
#include "../Texture.h"
#include "../GLSLProgram.h"

class Cube : public TriangleMesh, public Component
{
private:
	Texture* m_texture;

public:
	Cube(Texture* texture, GLfloat size);
	void Update();
	void Render(GLSLProgram* prog);
	void DrawUI();
};