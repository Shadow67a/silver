#ifndef APP_HPP
#define APP_HPP

#include <GLFW/glfw3.h>
#include <vector>

#include "./Lib.hpp"
#include "./View.hpp"

class silver::App
{
	private:
		View* _mainView;

	public:
		std::vector<GLFWwindow*> windows;

		App(int width, int height, const char* title);
		App &mainView(View view);
		App &run();
};

#endif
