#ifndef SPINNER_H
#define SPINNER_H

#include "Component.h"

class Spinner : public Component
{
private:
	glm::mat4* m_model;
	float m_speed;

public:
	Spinner(glm::mat4* model);
	void Update(float deltaTime);
	void Render(GLSLProgram* prog);
	void DrawUI();
};


#endif //SPINNER_H