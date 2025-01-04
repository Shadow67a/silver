#include <iostream>

#include "./glad.h"

#include "./Lib.hpp"
#include "./Shader.hpp"

silver::Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	int success;
	char log[512];

	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		std::cout << "[ERROR!]: Could not compile vertex shader!\n" << log << std::endl;
	};
	glComplileShader(fragmentShader);
	if (!success)
        {
                glGetShaderInfoLog(fragmentShader, 512, NULL, log);
                std::cout << "[ERROR!]: Could not compile fragment shader!\n" << log << std::endl;
        };

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, log);
		std::cout << "[ERROR!]: Could not link shader program!\n" << log << std::endl;
	};

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
};

silver::Shader::use()
{
	glUseProgram(shaderProgram);
};
