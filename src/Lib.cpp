#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Lib.hpp"

glm::vec3 silver::hex(int color)
{
	return glm::vec3((float)(color>>16 & 0xff)/255, (float)(color>>8 & 0xff)/255, (float)(color & 0xff)/255);
};
