#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include <map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Lib.hpp"
#include "./View.hpp"
#include "./Shader.hpp"

class silver::Triangle : public silver::View
{
	private:
		Shader* shader;
	public:
		unsigned int VAO;
		unsigned int VBO;

		Triangle();
		void render() override;
};

class silver::Quad : public silver::View
{
        private:
                Shader* shader;
        public:
                unsigned int VAO;
                unsigned int VBO;

                Quad();
                void render() override;
};

class silver::Image : public silver::View
{
	private:
		Shader* shader;

	public:
		unsigned int VAO;
		unsigned int VBO;
		unsigned int texture;
		int width, height;

		Image(const char* path);
		void render() override;
};
#endif
