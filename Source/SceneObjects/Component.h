#ifndef COMPONENT_H
#define COMPONENT_H

#include "../GLSLProgram.h"

class Component 
{
public:
	virtual void Update(float deltaTime) = 0;
	virtual void Render(GLSLProgram* prog) = 0;

	virtual void DrawUI() = 0;
};

#endif //COMPONENT_H
