#ifndef RUNNER_H
#define RUNNER_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scenes/Scene.h"
#include "SystemInformation.h"
#include "stb/stb_image.h"

class Runner 
{
public:
	Runner(std::string name, int width, int height)
	{
		if (!glfwInit())
		{
			exit(EXIT_FAILURE);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		if (!window)
		{
			std::cerr << "UNABLE TO CREATE OPENGL CONTEXT" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);

		int iWidth, iHeight;
		int channels;
		unsigned char* iPixels = stbi_load("Media/Images/GreyfishEngine.png", &iWidth, &iHeight, &channels, 4);

		GLFWimage images[1];
		images[0].width = iWidth;
		images[0].height = iHeight;
		images[0].pixels = iPixels;
		glfwSetWindowIcon(window, 1, images);

		glfwGetFramebufferSize(window, &m_fbw, &m_fbh);

		if (!gladLoadGL())
		{
			exit(-1);
		}

		SystemInformation::OutputInformation();

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	}

	int Run(Scene& scene)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		scene.SetDimensions(m_fbw, m_fbh);
		scene.Start(window);
		scene.Resize(m_fbw, m_fbh);

		MainLoop(scene);

		scene.CleanUp();

		glfwTerminate();

		return EXIT_SUCCESS;
	}

private:
	GLFWwindow* window;
	int m_fbw, m_fbh;
	float m_deltaTime;
	float m_lastFrame;

	void MainLoop(Scene& scene)
	{
		while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSwapBuffers(window);
			glfwPollEvents();

			UpdateDeltaTime();
			scene.Render();
			scene.Update(window, m_deltaTime);
		}
	}

	void UpdateDeltaTime()
	{
		float currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;
	}
};

#endif #RUNNER_H