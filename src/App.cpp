#include <iostream>
#include <vector>
#include "./glad.h"
#include <GLFW/glfw3.h>

#include "./Lib.hpp"
#include "./App.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
};

silver::App::App(int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "[ERROR]: Failed to create window!" << std::endl;
		glfwTerminate();
		exit(-1);
	};
	windows.push_back(window);
	glfwMakeContextCurrent(windows[0]);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "[ERROR]: Failed to create window!" << std::endl;
		glfwTerminate();
		exit(-1);
	};

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(windows[0], framebufferSizeCallback);
};

silver::App &silver::App::mainView(View view)
{
	this->_mainView = &view;
	return *this;
};

silver::App &silver::App::run()
{
	while(!glfwWindowShouldClose(windows[0]))
	{
		glClearColor(this->_mainView->backgroundColor.r, this->_mainView->backgroundColor.g, this->_mainView->backgroundColor.b, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(windows[0]);
		glfwPollEvents();
	};
	glfwTerminate();
	return *this;
};
