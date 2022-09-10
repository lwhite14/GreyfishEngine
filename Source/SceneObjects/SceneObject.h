#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Component.h"
#include "../GLSLProgram.h"
#include "Dimensions.h"

class SceneObject 
{
public:
	SceneObject(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, GLSLProgram* prog = nullptr);
	void Update(float deltaTime);
	void Render(glm::mat4& view, glm::mat4& projection);
	void DrawHeaderUI();
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	void SetModel(glm::mat4* modelMatrix);
	void SetDimensions(Dimensions* dimensions);

	std::string GetName();
	glm::mat4 GetModel();
	glm::mat4* GetModelPtr();
	std::vector<Component*> GetComponents();
	std::vector<Component*> GetAllComponents();
	Dimensions* GetDimensions();
	GLSLProgram* GetGLSLProgram();

private:
	std::string m_name;
	char m_nameArr[64];
	GLSLProgram* m_prog;
	glm::mat4 m_model;
	std::vector<Component*> m_components;
	Dimensions* m_dimensions;

	static void SetMatrices(GLSLProgram* prog, glm::mat4& view, glm::mat4& model, glm::mat4& projection);
};

#endif //SCENEOBJECT_H