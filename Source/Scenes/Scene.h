#ifndef SCENE_H
#define SCENE_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Scene
{
protected:
	int m_width, m_height;
	glm::mat4 m_view, m_projection;

public:
	virtual void Start(GLFWwindow* window) = 0;
	virtual void Update(GLFWwindow* window, float deltaTime) = 0;
	virtual void Render() = 0;
	void Resize(float w, float h)
	{
		glViewport(0, 0, w, h);
		m_width = w;
		m_height = h;
		m_projection = glm::perspective(glm::radians(80.0f), (float)w / h, 0.3f, 1000.0f);
	}
	void SetDimensions(int w, int h)
	{
		m_width = w;
		m_height = h;
	}
	virtual void CleanUp() = 0;
};
#endif //SCENE_H