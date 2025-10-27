#include <iostream>
#include <map>
#include <string>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "./glad.h"

#include "./Lib.hpp"
#include "./Text.hpp"

silver::Font::Font()
{
	//do nothing;
};

silver::Font::Font(const char* path)
{
	if (FT_New_Face(silver::ft, path, 0, &face))
	{
		std::cout << "[ERROR]: Failed to load font: " << path << std::endl;
	};

	FT_Set_Pixel_Sizes(face, 0, 64);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "[ERROR]: Failed to load glyph: " << c << std::endl;
		};

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	};
};

void silver::Font::resize(int size)
{
	FT_Set_Pixel_Sizes(face, 0, size);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (auto &pair : Characters) {
		glDeleteTextures(1, &pair.second.textureID);
	};
	Characters.clear();

        for (unsigned char c = 0; c < 128; c++)
        {
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                        std::cout << "[ERROR]: Failed to load glyph: " << c << std::endl;
                };

                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RED,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                Character character = {
                        texture,
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        face->glyph->advance.x
                };
                Characters.insert(std::pair<char, Character>(c, character));
        };
};

silver::Text::Text(std::string text)
{
	shader = &textShader;

	_text = text;
	_font = arial;

	fillColor = glm::vec3(0.0, 0.0, 0.0);

	size = 64.0;

	_position = glm::vec3(0.0, 0.0, 0.0);
        _rotation = glm::vec3(0.0, 0.0, 0.0);
        _scale = glm::vec3(1.0, 1.0, 1.0);

	_parent = nullptr;

	textWidth = 0.0;
	textHeight = 0.0;
	char previous = 0;
	std::string::const_iterator c;
	for (c = _text.begin(); c != _text.end(); c++)
        {
                Character ch = _font.Characters[*c];

                if (previous && _font.face && FT_HAS_KERNING(_font.face)) {
                        FT_Vector delta;
                        FT_Get_Kerning(_font.face, previous, *c, FT_KERNING_DEFAULT, &delta);
                        textWidth += (delta.x >> 6);
                };
		textWidth += (ch.advance >> 6);
		textHeight = std::max(static_cast<float>(ch.size.y), textHeight);
		previous = *c;
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
};

silver::Text *silver::Text::font(silver::Font font)
{
	_font = font;
	_font.resize((int)size);

	textWidth = 0.0;
	textHeight = 0.0;
        char previous = 0;
        std::string::const_iterator c;
        for (c = _text.begin(); c != _text.end(); c++)
        {
                Character ch = _font.Characters[*c];

                if (previous && _font.face && FT_HAS_KERNING(_font.face)) {
                        FT_Vector delta;
                        FT_Get_Kerning(_font.face, previous, *c, FT_KERNING_DEFAULT, &delta);
                        textWidth += (delta.x >> 6);
                };
                textWidth += (ch.advance >> 6);
		textHeight = std::max(static_cast<float>(ch.size.y), textHeight);
                previous = *c;
        };

	return this;
};

silver::Text *silver::Text::text(std::string text)
{
	_text = text;
	return this;
};

silver::Text *silver::Text::fill(glm::vec3 color)
{
	fillColor = color;
	return this;
};

silver::Text *silver::Text::scale(float size)
{
	this->size = size;
	_font.resize((int)size);

	textWidth = 0.0;
	textHeight = 0.0;
        char previous = 0;
        std::string::const_iterator c;
        for (c = _text.begin(); c != _text.end(); c++)
        {
                Character ch = _font.Characters[*c];

                if (previous && _font.face && FT_HAS_KERNING(_font.face)) {
                        FT_Vector delta;
                        FT_Get_Kerning(_font.face, previous, *c, FT_KERNING_DEFAULT, &delta);
                        textWidth += (delta.x >> 6);
                };
                textWidth += (ch.advance >> 6);
		textHeight = std::max(static_cast<float>(ch.size.y), textHeight);
                previous = *c;
        };

	return this;
};

void silver::Text::hoverHandler()
{
	//todo;
};

void silver::Text::render()
{
	bool p = _parent != nullptr;
	float aspect = (float)silver::windowWidth/(float)silver::windowHeight;

	shader->use();
	shader->setVec3("color", fillColor);
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(_position.x+(p ? _parent->_position.x*-2.41 : 0.0), _position.y+(p ? _parent->_position.y*2.41 : 0.0), _position.z));
        modelMatrix = glm::rotate(modelMatrix, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = glm::rotate(modelMatrix, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0, 1.0, 1.0));

//	glm::mat4 orthoProjection = glm::ortho(0.0f, (float)silver::windowWidth, 0.0f, (float)silver::windowHeight);
	glm::mat4 orthoProjection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
	shader->setMat4("modelMatrix", modelMatrix);
	shader->setMat4("projectionMatrix", orthoProjection);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

//	float scale = (float)silver::windowHeight/1080.0;
//	float x = ((float)silver::windowWidth/2.0)-(textWidth*scale/2.0);
//	float y = ((float)silver::windowHeight/2.0)-(textHeight*scale/2.0);
	float scale = 0.002;
	float x = -textWidth*scale/2.0;
	float y = -textHeight*scale/2.0;

	char previous = 0;
	std::string::const_iterator c;
	for (c = _text.begin(); c != _text.end(); c++)
	{
		Character ch = _font.Characters[*c];

		if (previous && _font.face && FT_HAS_KERNING(_font.face)) {
        		FT_Vector delta;
        		FT_Get_Kerning(_font.face, previous, *c, FT_KERNING_DEFAULT, &delta);
        		x += (delta.x >> 6) * scale;
   		};

		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
        	float h = ch.size.y * scale;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
        	};

		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (float)(ch.advance >> 6) * scale;
		previous = *c;
	};
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
};

silver::Font arial = silver::Font();
