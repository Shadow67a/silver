#ifndef SHADER_HPP
#define SHADER_HPP

#include "./glad.h"

#include "./Lib.hpp"

class silver::Shader
{
	private:
		unsigned int program;
		unsigned int vertexShader;
		unsigned int fragmentShader;
	public:
		Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
		void use();
};

