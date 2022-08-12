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

	void CompileShaders();
public:
	BasicScene();
	void Start(GLFWwindow* window);
	void Update(GLFWwindow* window, float deltaTime);
	void Render();
	void CleanUp();
};

#endif