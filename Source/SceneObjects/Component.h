#ifndef COMPONENT_H
#define COMPONENT_H

#include <yaml-cpp/yaml.h>
#include "../GLSLProgram.h"

class Component 
{
protected:
	std::string m_name;
	bool m_interfaceOpen;

public:
	Component(std::string name) :
		m_name{ name },
		m_interfaceOpen{ true }
	{}

	virtual void Update(float deltaTime) = 0;
	virtual void Render(GLSLProgram* prog) = 0;
	virtual void DrawUI() = 0;
	virtual void Serialization(YAML::Emitter& out) = 0;

	std::string GetName() { return m_name; }
	bool GetInterfaceOpen() { return m_interfaceOpen; }

	void SetName(std::string name) { m_name = name; }
	void SetInterfaceOpen(bool interfaceOpen) { m_interfaceOpen = interfaceOpen; }

};

#endif //COMPONENT_H
