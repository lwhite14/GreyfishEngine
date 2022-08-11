#ifndef BASICSCENE_H
#define BASICSCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "../GLSLProgram.h"
#include "../Texture.h"
#include "../Drawables/Cube.h"

class BasicScene : public Scene 
{
private:
	GLSLProgram m_prog;
	Cube m_cube;
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