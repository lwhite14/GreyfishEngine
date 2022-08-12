#include "SceneObject.h"

SceneObject::SceneObject()
{

}

void SceneObject::Update()
{
	for (unsigned int i = 0; i < m_components.size(); i++) 
	{
		m_components[i]->Update();
	}
}

void SceneObject::Render()
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Render();
	}
}

void SceneObject::AddComponent(Component* component)
{
	m_components.push_back(component);
}
