#include "SceneObject.h"

#include <iostream>
#include "../Drawables/Drawable.h"
#include "../Drawables/ObjMesh.h"
#include "../Drawables/Cube.h"

SceneObject::SceneObject() : m_position{0.0f, 0.0f, 0.0f}
{

}

SceneObject::SceneObject(glm::vec3 position) : m_position{ position }
{

}

void SceneObject::Update()
{
	for (unsigned int i = 0; i < m_components.size(); i++) 
	{
		m_components[i]->Update();
	}
}

void SceneObject::Render(GLSLProgram* prog, glm::mat4& view, glm::mat4& projection)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_position);
	model = model * m_model;

	prog->Use();
	SetMatrices(prog, view, model, projection);
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Render();
	}
}

void SceneObject::SetMatrices(GLSLProgram* prog, glm::mat4& view, glm::mat4& model, glm::mat4& projection)
{
	glm::mat4 mv = view * model;
	prog->SetUniform("MVP", projection * mv);
	prog->SetUniform("ModelViewMatrix", mv);
	prog->SetUniform("NormalMatrix", glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
}

void SceneObject::AddComponent(Component* component)
{
	bool drawableComponenet = false;
	for (unsigned int i = 0; i < m_components.size(); i++) 
	{
		if (typeid(*m_components[i]) == typeid(Drawable) 
			|| typeid(*m_components[i]) == typeid(ObjMesh)
			|| typeid(*m_components[i]) == typeid(Cube))
		{
			drawableComponenet = true;
		}
	}

	if (drawableComponenet) 
	{
		std::cout << "Unable to add component: Another drawable already on SceneObject" << std::endl;
	}
	else 
	{
		m_components.push_back(component);
	}
}

void SceneObject::SetPosition(glm::vec3* position) 
{
	m_position = *position;
}

void SceneObject::SetModel(glm::mat4* modelMatrix) 
{
	m_model = *modelMatrix;
}
