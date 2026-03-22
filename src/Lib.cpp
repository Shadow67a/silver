#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include "./Lib.hpp"
#include "./Animation.hpp"
#include "./State.hpp"

glm::mat4 silver::view = glm::mat4(1.0);
glm::mat4 silver::projection = glm::mat4(1.0);

glm::mat4 silver::invView = glm::mat4(1.0);
glm::mat4 silver::invProjection = glm::mat4(1.0);

int silver::windowWidth = 0;
int silver::windowHeight = 0;

float silver::iTime = 0.0;

FT_Library silver::ft;

GLFWwindow* silver::currentWindow = nullptr;

std::vector<silver::State*> silver::states = {};

Mouse silver::mouse;

void Mouse::update()
{
		double mouseX = 0.0;
                double mouseY = 0.0;
                glfwGetCursorPos(silver::currentWindow, &mouseX, &mouseY);
                //NDC;
                mouseX = (2.0*mouseX/silver::windowWidth)-1.0;
                mouseY = 1.0-(2.0*mouseY/silver::windowHeight);
                cursor = glm::vec4(mouseX, mouseY, -1.0, 1.0);
                //cursor = cursor * silver::invProjection * silver::invView;
		cursor = silver::invView * silver::invProjection * cursor;
                mouseDown = glfwGetMouseButton(silver::currentWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
                clicked = !mouseDown && mouseDownPrev;
                dragStart = !mouseDownPrev && mouseDown;
                dragged = (mouseDown && mouseDownPrev);
		GLFWcursor* cursor = glfwCreateStandardCursor(_shape);
        	glfwSetCursor(silver::currentWindow, cursor);
};

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

void Mouse::cursorShape(int shape)
{
	_shape = shape;
};
