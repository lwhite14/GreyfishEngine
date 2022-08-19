#pragma once

#include "TriangleMesh.h"
#include "../SceneObjects/Component.h"

class Cube : public TriangleMesh, public Component
{
public:
	Cube(GLfloat size = 1.0f);
	void Update();
	void Render();
	void DrawUI();
};