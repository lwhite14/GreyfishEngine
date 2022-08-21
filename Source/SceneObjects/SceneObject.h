#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Component.h"
#include "Dimensions.h"
#include "../GLSLProgram.h"

class SceneObject 
{
public:
	SceneObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void Update();
	void Render(GLSLProgram* prog, glm::mat4& view, glm::mat4& projection);
	void AddComponent(Component* component);

	void SetModel(glm::mat4* modelMatrix);
	void SetDimensions(Dimensions* dimensions);

	glm::mat4 GetModel();
	std::vector<Component*> GetComponents();
	std::vector<Component*> GetAllComponents();
	Dimensions* GetDimensions();

private:
	GLSLProgram m_prog;
	glm::mat4 m_model;
	std::vector<Component*> m_components;
	Dimensions* m_dimensions;

	void SetMatrices(GLSLProgram* prog, glm::mat4& view, glm::mat4& model, glm::mat4& projection);
};

#endif //SCENEOBJECT_H