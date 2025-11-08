#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "./glad.h"
#include <GLFW/glfw3.h>

#include "./Lib.hpp"
#include "./App.hpp"
#include "./Shader.hpp"
#include "./Text.hpp"
#include "./View.hpp"

silver::App* app;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	silver::projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
	silver::windowWidth = width;
	silver::windowHeight = height;
	glViewport(0, 0, width, height);
	app->_mainView->resize();
};

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button==GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS)
	{
		silver::clicked = 1;
	};
}

silver::App::App(int width, int height, const char* title)
{
	app = this;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	silver::windowWidth = width;
	silver::windowHeight = height;
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "[ERROR]: Failed to create window!" << std::endl;
		glfwTerminate();
		exit(-1);
	};
	windows.push_back(window);
	glfwMakeContextCurrent(windows[0]);
	silver::currentWindow = windows[0];

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "[ERROR]: Failed to initialize opengl context!" << std::endl;
		glfwTerminate();
		exit(-1);
	};

	glViewport(0, 0, width, height);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetFramebufferSizeCallback(windows[0], framebufferSizeCallback);
	glfwSetMouseButtonCallback(windows[0], mouseButtonCallback);

	view = glm::lookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	silver::projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
	flatShaded = silver::Shader(flatShadedVertexSource, flatShadedFragmentSource);
	imageShader = silver::Shader(imageVertexSource, imageFragmentSource);
	textShader = silver::Shader(textVertexSource, textFragmentSource);
	viewShader = silver::Shader(viewVertexSource, imageFragmentSource);

	if (FT_Init_FreeType(&silver::ft))
        {
                std::cout << "[ERROR!]: Failed to init freetype!" << std::endl;
        };

	arial = silver::Font("./assets/arial.ttf");
};

silver::App &silver::App::mainView(View* view)
{
	this->_mainView = view;
	return *this;
};

silver::App &silver::App::run()
{
	while(!glfwWindowShouldClose(windows[0]))
	{
		silver::iTime = static_cast<float>(glfwGetTime());
		double mouseX = 0.0;
		double mouseY = 0.0;
		glfwGetCursorPos(windows[0], &mouseX, &mouseY);
		//NDC;
		mouseX = (2.0*mouseX/windowWidth)-1.0;
		mouseY = 1.0-(2.0*mouseY/windowHeight);
		silver::cursor = glm::vec4(mouseX, mouseY, -1.0, 1.0);
		glm::mat4 invProj = glm::inverse(silver::projection);
		glm::mat4 invView = glm::inverse(silver::view);
		silver::cursor = silver::cursor * invProj * invView;

		this->_mainView->render();

		glfwSwapBuffers(windows[0]);
		glfwPollEvents();
	};
	FT_Done_FreeType(silver::ft);
	glfwTerminate();
	return *this;
};
