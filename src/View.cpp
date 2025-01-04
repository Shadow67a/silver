#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Lib.hpp"
#include "./View.hpp"

silver::View &silver::View::background(glm::vec3 backgroundColor)
{
	this->backgroundColor = backgroundColor;
	return *this;
};
