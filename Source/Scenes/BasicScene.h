#ifndef BASICSCENE_H
#define BASICSCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "../GLSLProgram.h"
#include "../Texture.h"
#include "../SceneObjects/SceneObject.h"
#include "../SceneObjects/CameraGameView.h"
#include "../UserInterface/MasterUI.h"
#include "../Framebuffer.h"
#include "../MasterShaders.h"

class BasicScene : public Scene 
{
private:
	MasterShaders m_masterShaders;
	SceneObject* m_cubeObject;
	MasterUI m_masterUI;
	CameraGameView* m_cameraGameView;
	Framebuffer m_framebuffer;

public:
	BasicScene();
	void Start(GLFWwindow* window);
	void Update(GLFWwindow* window, float deltaTime);
	void Render();
	void CleanUp();
};

#endif