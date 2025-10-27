#include <iostream>
#include <string>
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

#include "./glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "./Lib.hpp"
#include "./Elements.hpp"
#include "./Vertices.hpp"
#include "./Shader.hpp"

silver::Triangle::Triangle()
{
	x = glm::vec4(1.0, 1.0, 1.0, 1.0);
	y = glm::vec4(1.0, 1.0, 1.0, 1.0);
	_position = glm::vec3(0.0, 0.0, 0.0);
	_rotation = glm::vec3(0.0, 0.0, 0.0);
	_scale = glm::vec3(0.5, 0.5, 0.5);
	//Border for triangle not implemented but we initialize this anyways;
	borderWidth = 0.0;
	bx = glm::vec4(1.0, 1.0, 1.0, 1.0);
	by = glm::vec4(1.0, 1.0, 1.0, 1.0);
	_cornerRadius = 0.0;
	_parent = nullptr;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)( 3*sizeof(float) ));
        glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader = &flatShaded;
};

void silver::Triangle::render()
{
	shader->use();

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(_position.x+(_parent != nullptr ? +_parent->_position.x : 0.0), _position.y+(_parent != nullptr ? +_parent->_position.y : 0.0), _position.z+(_parent != nullptr ? +_parent->_position.z : 0.0)));
	modelMatrix = glm::rotate(modelMatrix, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::rotate(modelMatrix, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, _scale);

	shader->setMat4("modelMatrix", modelMatrix);
	shader->setMat4("viewMatrix", silver::view);
        shader->setMat4("projectionMatrix", silver::projection);
	shader->setVec4("x", x);
	shader->setVec4("y", y);
	shader->setMat4("gradientTransform", gradientTransform);
	shader->setVec4("bx", x);
        shader->setVec4("by", y);
        shader->setMat4("bGradientTransform", gradientTransform);
        shader->setFloat("borderWidth", borderWidth);
        shader->setFloat("cornerRadius", _cornerRadius);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
};

silver::Quad::Quad()
{
        x = glm::vec4(1.0, 1.0, 1.0, 1.0);
        y = glm::vec4(1.0, 1.0, 1.0, 1.0);
        _position = glm::vec3(0.0, 0.0, 0.0);
        _rotation = glm::vec3(0.0, 0.0, 0.0);
        _scale = glm::vec3(0.5, 0.5, 0.5);
	borderWidth = 0.0;
        bx = glm::vec4(1.0, 1.0, 1.0, 1.0);
        by = glm::vec4(1.0, 1.0, 1.0, 1.0);
	_cornerRadius = 0.0;
	_parent = nullptr;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)) );
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        shader = &flatShaded;
};

void silver::Quad::render()
{
        shader->use();

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(_position.x+(_parent != nullptr ? +_parent->_position.x : 0.0), _position.y+(_parent != nullptr ? +_parent->_position.y : 0.0), _position.z+(_parent != nullptr ? +_parent->_position.z : 0.0)));
        modelMatrix = glm::rotate(modelMatrix, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = glm::rotate(modelMatrix, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, _scale);

        shader->setMat4("modelMatrix", modelMatrix);
	shader->setMat4("viewMatrix", silver::view);
	shader->setMat4("projectionMatrix", silver::projection);
        shader->setVec4("x", x);
	shader->setVec4("y", y);
	shader->setMat4("gradientTransform", gradientTransform);
        shader->setVec4("bx", bx);
        shader->setVec4("by", by);
        shader->setMat4("bGradientTransform", bGradientTransform);
	shader->setFloat("borderWidth", borderWidth);
	shader->setFloat("cornerRadius", _cornerRadius);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
};

silver::Image::Image(const char* path)
{
	x = glm::vec4(1.0, 1.0, 1.0, 1.0);
        y = glm::vec4(1.0, 1.0, 1.0, 1.0);
	_position = glm::vec3(0.0, 0.0, 0.0);
        _rotation = glm::vec3(0.0, glm::radians(180.0), 0.0);
        _scale = glm::vec3(0.5, 0.5, 0.5);
	borderWidth = 0.0;
        bx = glm::vec4(1.0, 1.0, 1.0, 1.0);
        by = glm::vec4(1.0, 1.0, 1.0, 1.0);
	_cornerRadius = 0.0;
	_parent = nullptr;

	int nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "[ERROR!]: Failed to load image: " << path << std::endl;
	};
	stbi_image_free(data);

	glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)) );
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

	shader = &imageShader;
};

void silver::Image::render()
{
	shader->use();

        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(_position.x+(_parent != nullptr ? +_parent->_position.x : 0.0), _position.y+(_parent != nullptr ? +_parent->_position.y : 0.0), _position.z+(_parent != nullptr ? +_parent->_position.z : 0.0)));
        modelMatrix = glm::rotate(modelMatrix, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = glm::rotate(modelMatrix, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, _scale);

        shader->setMat4("modelMatrix", modelMatrix);
        shader->setMat4("viewMatrix", silver::view);
        shader->setMat4("projectionMatrix", silver::projection);
	shader->setVec4("bx", bx);
        shader->setVec4("by", by);
        shader->setMat4("bGradientTransform", bGradientTransform);
	shader->setFloat("borderWidth", borderWidth);
	shader->setFloat("cornerRadius", _cornerRadius);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
};
