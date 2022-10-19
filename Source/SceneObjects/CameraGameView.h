#ifndef CAMERAGAMEVIEW_H
#define CAMERAGAMEVIEW_H

#include <glm/glm.hpp>
#include "../UserInterface/MasterUI.h"

class CameraGameView 
{
private:
	glm::vec3 m_cameraPos; // = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_cameraFront; // = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 m_cameraUp; // = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 m_cameraRight; // = glm::vec3(1.0, 0.0, 0.0);
	float m_lastX;
	float m_lastY;
	float m_yaw;
	float m_pitch;
	bool isFirst;

public:
	CameraGameView(int width, int height);
	void Update(float deltaTime, MasterUI* masterUI, glm::mat4& view);

};

#endif //CAMERAGAMEVIEW_H