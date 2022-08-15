#ifndef BASICSCENE_H
#define BASICSCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "../GLSLProgram.h"
#include "../Texture.h"
#include "../SceneObjects/SceneObject.h"
#include "../UserInterface/MasterUI.h"

class BasicScene : public Scene 
{
private:
	GLSLProgram m_prog;
	SceneObject m_cubeObject;
	MasterUI m_masterUI;

	unsigned int m_framebuffer;
	unsigned int m_textureColorbuffer;

	glm::vec3 m_cameraPos; // = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_cameraFront; // = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 m_cameraUp; // = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 m_cameraRight; // = glm::vec3(1.0, 0.0, 0.0);
	float m_lastX;
	float m_lastY;
	float m_yaw;
	float m_pitch;

	void CompileShaders();
public:
	BasicScene();
	void Start(GLFWwindow* window);
	void Update(GLFWwindow* window, float deltaTime);
	void Render();
	void CleanUp();
};

#endif