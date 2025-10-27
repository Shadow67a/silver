#ifndef TEXT_HPP
#define TEXT_HPP

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Lib.hpp"
#include "./View.hpp"
#include "./Shader.hpp"

struct Character
{
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

struct silver::Font
{
	Font();
	Font(const char* path);
	void resize(int size);
	FT_Face face;
	std::map<char, Character> Characters;
};

class silver::Text : public silver::View
{
	private:
		silver::Shader* shader;
	public:
		unsigned int VAO;
		unsigned int VBO;

		glm::vec3 fillColor;

		float size;
		float textWidth;
		float textHeight;

		std::string _text;
		silver::Font _font;

		Text(std::string text);

		silver::Text *font(silver::Font font);
		silver::Text *text(std::string text);
		silver::Text *fill(glm::vec3 color);
		silver::Text *scale(float size);

		void hoverHandler() override;

		void render() override;
};

extern silver::Font arial;
#endif
