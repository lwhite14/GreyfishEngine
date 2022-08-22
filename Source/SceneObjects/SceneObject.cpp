#include "SceneObject.h"

#include <iostream>
#include "../Drawables/Drawable.h"
#include "../Drawables/ObjMesh.h"
#include "../Drawables/Cube.h"
#include "Dimensions.h"
#include "../MasterShaders.h"
#include "../Dependencies/imgui/imgui.h"

SceneObject::SceneObject(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, GLSLProgram* prog) :
	m_name{ name },
	m_model{ glm::mat4(1.0f) },
	m_prog{ prog }
{
	m_dimensions = new Dimensions(position, rotation, scale);
	if (prog == nullptr) 
	{
		m_prog = MasterShaders::shaderList[0];
	}
}

void SceneObject::Update()
{
	for (unsigned int i = 0; i < m_components.size(); i++) 
	{
		m_components[i]->Update();
	}
}

void SceneObject::Render(glm::mat4& view, glm::mat4& projection)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_dimensions->GetPosition());
	model = glm::rotate(model, glm::radians(m_dimensions->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_dimensions->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_dimensions->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, m_dimensions->GetScale());
	model = model * m_model;

	m_prog->Use();
	SetMatrices(m_prog, view, model, projection);
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Render(m_prog);
	}
}

void SceneObject::SetMatrices(GLSLProgram* prog, glm::mat4& view, glm::mat4& model, glm::mat4& projection)
{
	glm::mat4 mv = view * model;
	prog->SetUniform("MVP", projection * mv);
	prog->SetUniform("ModelViewMatrix", mv);
	prog->SetUniform("NormalMatrix", glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
}

void SceneObject::DrawHeaderUI() 
{
	ImGui::BeginChild("SceneObject", ImVec2(ImGui::GetContentRegionAvail().x, 60), true);
	ImGui::Text(m_name.c_str());
	if (ImGui::BeginCombo("Shader", m_prog->GetName().c_str()))
	{
		for (int i = 0; i < MasterShaders::shaderList.size(); i++)
		{
			//if (ImGui::Button(MasterShaders::shaderList[i]->GetName().c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f))) 
			if (ImGui::Selectable(MasterShaders::shaderList[i]->GetName().c_str()))
			{
				m_prog = MasterShaders::shaderList[i];
			}
		}
		ImGui::EndCombo();
	}
	ImGui::EndChild();
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
