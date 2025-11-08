#ifndef APP_HPP
#define APP_HPP

#include "./glad.h"
#include <GLFW/glfw3.h>
#include <vector>

#include "./Lib.hpp"
#include "./View.hpp"
#include "./Shader.hpp"

class silver::App
{
	public:
		View* _mainView;

		std::vector<GLFWwindow*> windows;

		App(int width, int height, const char* title);
		App &mainView(View* view);
		App &run();
};

#endif
