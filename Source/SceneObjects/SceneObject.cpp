#include "SceneObject.h"

#include <iostream>
#include "../Drawables/Drawable.h"
#include "../Drawables/ObjMesh.h"
#include "../Drawables/Cube.h"
#include "Dimensions.h"

SceneObject::SceneObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : 
	m_model{ glm::mat4(1.0f) }
{
	m_dimensions = new Dimensions(position, rotation, scale);
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
	model = glm::translate(model, m_dimensions->GetPosition());
	model = glm::rotate(model, glm::radians(m_dimensions->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_dimensions->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_dimensions->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, m_dimensions->GetScale());
	model = model * m_model;

	prog->Use();
	prog->SetUniform("Light.Position", glm::vec4(5.0f, 5.0f, 5.0f, 1.0f));
	prog->SetUniform("Light.La", glm::vec3(0.6f));
	prog->SetUniform("Light.Ld", glm::vec3(0.85f, 0.2f, 0.2f));
	prog->SetUniform("Light.Ls", glm::vec3(1.0f));
	prog->SetUniform("Material.Ka", glm::vec3(0.5f, 0.25f, 0.25f));
	prog->SetUniform("Material.Kd", glm::vec3(0.5f, 0.25f, 0.25f));
	prog->SetUniform("Material.Ks", glm::vec3(0.5f));
	prog->SetUniform("Material.Shininess", 64.0f);
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

void SceneObject::SetModel(glm::mat4* modelMatrix) { m_model = *modelMatrix; }
void SceneObject::SetDimensions(Dimensions* dimensions) { m_dimensions = dimensions; }

glm::mat4 SceneObject::GetModel() { return m_model; }
std::vector<Component*> SceneObject::GetComponents() { return m_components; }
std::vector<Component*> SceneObject::GetAllComponents() 
{
	std::vector<Component*> tempComponents = GetComponents();
	tempComponents.push_back(GetDimensions());
	std::rotate(tempComponents.rbegin(), tempComponents.rbegin() + 1, tempComponents.rend());
	return tempComponents;
}
Dimensions* SceneObject::GetDimensions() { return m_dimensions; }
