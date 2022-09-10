#ifndef COMPONENT_H
#define COMPONENT_H

#include <yaml-cpp/yaml.h>
#include "../GLSLProgram.h"

class Component 
{
private:
	std::string m_name;

public:
	Component(std::string name) :
		m_name{ name } 
	{}

	virtual void Update(float deltaTime) = 0;
	virtual void Render(GLSLProgram* prog) = 0;
	virtual void DrawUI() = 0;
	virtual void Serialization(YAML::Emitter& out) = 0;

	std::string GetName() { return m_name; }
};

#endif //COMPONENT_H
