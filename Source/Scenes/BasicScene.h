#ifndef BASICSCENE_H
#define BASICSCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "../GLSLProgram.h"
#include "../Texture.h"
#include "../SceneObjects/SceneObject.h"
#include "../UserInterface/Framerate.h"

class BasicScene : public Scene 
{
private:
	GLSLProgram m_prog;
	SceneObject m_cubeObject;
	Framerate m_framerate;
	GLuint m_tex1, m_tex2;

	void CompileShaders();
public:
	BasicScene();
	void Start(GLFWwindow* window);
	void Update(GLFWwindow* window, float deltaTime);
	void Render();
	void CleanUp();
};

#endif