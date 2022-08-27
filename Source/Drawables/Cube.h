#pragma once

#include "TriangleMesh.h"
#include "../SceneObjects/Component.h"
#include "../Texture.h"
#include "../GLSLProgram.h"
#include "../SceneObjects/SceneObject.h"

class Cube : public TriangleMesh, public Component
{
private:
	SceneObject* m_associatedObject;
	Texture* m_texture;
	glm::vec3 m_matAmbient;
	glm::vec3 m_matDiffuse;
	glm::vec3 m_matSpecular;
	float m_matShininess;

	float m_matAmbientArr[3];
	float m_matDiffuseArr[3];
	float m_matSpecularArr[3];
	float m_matShininessArr;

public:
	Cube(Texture* texture, GLfloat size, SceneObject* sceneObject);
	void Update(float deltaTime);
	void Render(GLSLProgram* prog);
	void DrawUI();

	void SetMatAmbient(glm::vec3 matAmbient);
	void SetMatDiffuse(glm::vec3 matDiffuse);
	void SetMatSpecular(glm::vec3 matSpecular);
	void SetMatShininess(float matShininess);
};