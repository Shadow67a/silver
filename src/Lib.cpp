#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include "./Lib.hpp"
#include "./Animation.hpp"
#include "./State.hpp"

glm::mat4 silver::view = glm::mat4(1.0);
glm::mat4 silver::projection = glm::mat4(1.0);

int silver::windowWidth = 0;
int silver::windowHeight = 0;

float silver::iTime = 0.0;

FT_Library silver::ft;

glm::vec4 silver::cursor = glm::vec4(1.0, 1.0, 1.0, 1.0);

GLFWwindow* silver::currentWindow = nullptr;

std::vector<silver::State*> silver::states = {};

bool silver::clicked = 0;
bool silver::hovered = 0;

glm::vec4 silver::hex(int color)
{
	return glm::vec4((float)(color>>16 & 0xff)/255, (float)(color>>8 & 0xff)/255, (float)(color & 0xff)/255, 1.0);
};

glm::vec4 silver::hex(int color, float opacity)
{
        return glm::vec4((float)(color>>16 & 0xff)/255, (float)(color>>8 & 0xff)/255, (float)(color & 0xff)/255, opacity);
};

Gradient silver::linearGradient(glm::vec4 x, glm::vec4 y, float angle)
{
	Gradient g;
	g.x = x;
	g.y = y;
	g.angle = angle;
	return g;
};

void silver::cursorShape(int shape)
{
	GLFWcursor* cursor = glfwCreateStandardCursor(shape);
	glfwSetCursor(currentWindow, cursor);
};
