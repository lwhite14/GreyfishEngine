#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Component.h"
#include "../GLSLProgram.h"

class SceneObject 
{
public:
	SceneObject();
	SceneObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 m_rotation);
	void Update();
	void Render(GLSLProgram* prog, glm::mat4& view, glm::mat4& projection);
	void AddComponent(Component* component);

	void SetPosition(glm::vec3* position);
	void SetRotation(glm::vec3* rotation);
	void SetScale(glm::vec3* scale);
	void SetModel(glm::mat4* modelMatrix);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
	glm::mat4 GetModel();

private:
	glm::mat4 m_model;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	std::vector<Component*> m_components;

	void SetMatrices(GLSLProgram* prog, glm::mat4& view, glm::mat4& model, glm::mat4& projection);
};

#endif //SCENEOBJECT_H