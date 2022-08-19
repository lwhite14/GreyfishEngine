#ifndef DIMENSIONS_H
#define DIMENSIONS_H

#include "Component.h"
#include <glm/glm.hpp>

class Dimensions : public Component
{
private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	float m_positionArr[3];
	float m_rotationArr[3];
	float m_scaleArr[3];

public:
	Dimensions(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void Update();
	void Render();
	void DrawUI();

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
};

#endif