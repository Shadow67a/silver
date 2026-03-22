#ifndef LIB_HPP
#define LIB_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define ARROW_CURSOR 0x00036001
#define HAND_CURSOR  0x00036004

#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1

struct Gradient
{
	glm::vec4 x;
	glm::vec4 y;
	float angle;
};

struct Mouse
{
        bool mouseDown;
        bool mouseDownPrev;
        bool dragged;
        bool dragStart;
        bool clicked;
        bool hovered;
	glm::vec4 cursor;

	void update();
        void cursorShape(int shape);
};

namespace silver
{
	class App;
	class View;
	class Shader;

	class Triangle;
	class Quad;
	class Image;
	struct Font;
	class Text;

	class State;
	class Animation;

	extern glm::mat4 view;
	extern glm::mat4 projection;
	extern glm::mat4 invView;
        extern glm::mat4 invProjection;
	extern int windowWidth;
	extern int windowHeight;
	extern float iTime;
	extern FT_Library ft;
	extern GLFWwindow* currentWindow;
	extern std::vector<State*> states;

	extern Mouse mouse;

	glm::vec4 hex(int color);
	glm::vec4 hex(int color, float opacity);
	Gradient linearGradient(glm::vec4 x, glm::vec4 y, float angle);
};

#endif
