#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Component.h"

class SceneObject 
{
public:
	SceneObject();
	void Update();
	void Render();
	void AddComponent(Component* component);

private:
	glm::mat4 m_model;
	glm::vec3 m_position;
	std::vector<Component*> m_components;
};

#endif //SCENEOBJECT_H