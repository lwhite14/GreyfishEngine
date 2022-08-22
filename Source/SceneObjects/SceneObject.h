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
	SceneObject(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, GLSLProgram* prog = nullptr);
	void Update();
	void Render(glm::mat4& view, glm::mat4& projection);
	void DrawHeaderUI();
	void AddComponent(Component* component);

	void SetModel(glm::mat4* modelMatrix);
	void SetDimensions(Dimensions* dimensions);

	std::string GetName();
	glm::mat4 GetModel();
	std::vector<Component*> GetComponents();
	std::vector<Component*> GetAllComponents();
	Dimensions* GetDimensions();

private:
	std::string m_name;
	GLSLProgram* m_prog;
	glm::mat4 m_model;
	std::vector<Component*> m_components;
	Dimensions* m_dimensions;

	static void SetMatrices(GLSLProgram* prog, glm::mat4& view, glm::mat4& model, glm::mat4& projection);
};

#endif //SCENEOBJECT_H