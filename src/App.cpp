#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

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
        silver::invProjection = glm::inverse(silver::projection);
	silver::windowWidth = width;
	silver::windowHeight = height;
	glViewport(0, 0, width, height);
	app->_mainView->resize();
};

silver::App::App(int width, int height, const char* title)
{
	app = this;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);

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
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetFramebufferSizeCallback(windows[0], framebufferSizeCallback);

	silver::view = glm::lookAt(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	silver::projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
	silver::invView = glm::inverse(silver::view);
        silver::invProjection = glm::inverse(silver::projection);
	flatShaded = silver::Shader(flatShadedVertexSource, flatShadedFragmentSource);
	imageShader = silver::Shader(imageVertexSource, imageFragmentSource);
	textShader = silver::Shader(textVertexSource, textFragmentSource);
	viewShader = silver::Shader(viewVertexSource, imageFragmentSource);

	if (FT_Init_FreeType(&silver::ft))
        {
                std::cout << "[ERROR!]: Failed to init freetype!" << std::endl;
        };

	arial = silver::Font("./assets/arial.ttf");
        stbi_set_flip_vertically_on_load(true);
};

silver::App &silver::App::mainView(View* view)
{
	this->_mainView = view;
	this->_mainView->mainView = 1;
	this->_mainView->body();
	return *this;
};

silver::App &silver::App::run()
{
	while(!glfwWindowShouldClose(windows[0]))
	{
		silver::iTime = static_cast<float>(glfwGetTime());

		silver::mouse.update();

		this->_mainView->render();

		silver::mouse.clicked = 0;
		silver::mouse.hovered = 0;
		silver::mouse.mouseDownPrev = silver::mouse.mouseDown;

		glfwSwapBuffers(windows[0]);
		glfwPollEvents();
		glfwSwapInterval(1);
	};
	FT_Done_FreeType(silver::ft);
	glfwTerminate();
	return *this;
};
