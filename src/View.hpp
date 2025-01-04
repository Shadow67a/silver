#ifndef VIEW_HPP
#define VIEW_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Lib.hpp"

class silver::View
{

	public:
		glm::vec3 backgroundColor;
		virtual View &background(glm::vec3 backgroundColor);
};

#endif
