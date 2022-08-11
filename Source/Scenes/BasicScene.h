#ifndef BASICSCENE_H
#define BASICSCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "../GLSLProgram.h"
#include "../Texture.h"

class BasicScene : public Scene 
{
private:
	GLSLProgram m_prog;
	GLuint m_tex1, m_tex2;
	unsigned int m_VBO, m_VAO;

	void CompileShaders();
public:
	BasicScene();
	void Start(GLFWwindow* window);
	void Update(GLFWwindow* window, float deltaTime);
	void Render();
	void CleanUp();
};

#endif