#include <iostream>

#include "./glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Lib.hpp"
#include "./Shader.hpp"

silver::Shader::Shader()
{
	//do nothing;
};

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
	glCompileShader(fragmentShader);
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

void silver::Shader::setInt(const char* uniform, int &value)
{
	glProgramUniform1i(program, glGetUniformLocation(program, uniform), value);
};

void silver::Shader::setFloat(const char* uniform, float &value)
{
        glProgramUniform1f(program, glGetUniformLocation(program, uniform), value);
};

void silver::Shader::setVec3(const char* uniform, glm::vec3 &value)
{
	glProgramUniform3fv(program, glGetUniformLocation(program, uniform), 1, glm::value_ptr(value));
};

void silver::Shader::setVec4(const char* uniform, glm::vec4 &value)
{
        glProgramUniform4fv(program, glGetUniformLocation(program, uniform), 1, glm::value_ptr(value));
};

void silver::Shader::setMat4(const char* uniform, glm::mat4 &value)
{
	glProgramUniformMatrix4fv(program, glGetUniformLocation(program, uniform), 1, GL_FALSE, glm::value_ptr(value));
};

void silver::Shader::use()
{
	glUseProgram(program);
};

const char* flatShadedVertexSource =
"#version 410 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 TexCoord;\n"
"uniform mat4 modelMatrix;\n"
"uniform mat4 viewMatrix;\n"
"uniform mat4 projectionMatrix;\n"
"out vec2 uv;\n"
"\n"
"void main()"
"{\n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);\n"
"    uv = TexCoord;\n"
"}\0";

const char* flatShadedFragmentSource =
"#version 410 core\n"
"out vec4 FragColor;\n"
"in vec2 uv;\n"
"uniform vec4 x;\n"
"uniform vec4 y;\n"
"uniform mat4 gradientTransform;\n"
"uniform vec4 bx;\n"
"uniform vec4 by;\n"
"uniform mat4 bGradientTransform;\n"
"uniform float borderWidth;\n"
"uniform float cornerRadius;\n"
"\n"
"float sdf(vec2 uv, vec2 size, float radius)\n"
"{\n"
"	vec2 d = abs(uv) - (size-radius);\n"
"	return length(max(d, 0.0))-radius;\n"
"}\n"
"void main()\n"
"{\n"
"	vec2 st = (uv * 2.0)-1.0\n;"
"	float bw = borderWidth * 0.02;\n"
"       float cr = cornerRadius * 2.0;\n"
"	vec4 innerTransform = gradientTransform * vec4(uv, 0.0f, 1.0f);\n"
"	float innerDist = sdf(st, vec2(1.0, 1.0)-bw, cr-bw);\n"
"	float outerDist = sdf(st, vec2(1.0, 1.0), cr);\n"
"	float aa = fwidth(outerDist);\n"
"	float innerMask = smoothstep(aa, -aa, innerDist);\n"
"	float outerMask = smoothstep(aa, -aa, outerDist);\n"
"	vec4 innerColor = mix(x, y, innerTransform.y) * innerMask;\n"
"	vec4 borderTransform = bGradientTransform * vec4(uv, 0.0f, 1.0f);\n"
"	vec4 borderColor = mix(bx, by, borderTransform.y) * max((outerMask - innerMask), 0.0);\n"
"	FragColor = innerColor + borderColor;\n"
"}\0";

const char* imageVertexSource =
"#version 410 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 TexCoord;\n"
"uniform mat4 modelMatrix;\n"
"uniform mat4 viewMatrix;\n"
"uniform mat4 projectionMatrix;\n"
"out vec2 uv;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);\n"
"       uv = TexCoord;\n"
"}\0";

const char* imageFragmentSource =
"#version 410 core\n"
"out vec4 FragColor;\n"
"in vec2 uv;\n"
"uniform sampler2D image;\n"
"uniform vec4 bx;\n"
"uniform vec4 by;\n"
"uniform mat4 bGradientTransform;\n"
"uniform float borderWidth;\n"
"uniform float cornerRadius;\n"
"uniform vec4 tint;"
"uniform vec4 fill;"
"\n"
"float sdf(vec2 uv, vec2 size, float radius)\n"
"{\n"
"       vec2 d = abs(uv) - (size-radius);\n"
"       return length(max(d, 0.0))+min(max(d.x, d.y),0.0)-radius;\n"
"}\n"
"void main()\n"
"{\n"
"       vec2 st = (uv * 2.0)-1.0;\n"
"       float bw = borderWidth * 0.02;\n"
"       float cr = cornerRadius * 2.0;\n"
"       float innerDist = sdf(st, vec2(1.0, 1.0)-bw, cr-bw);\n"
"       float outerDist = sdf(st, vec2(1.0, 1.0), cr);\n"
"       float aa = fwidth(outerDist);\n"
"       float innerMask = smoothstep(aa, -aa, innerDist);\n"
"       float outerMask = smoothstep(aa, -aa, outerDist);\n"
"       vec4 innerColor = texture(image, uv) * innerMask;\n"
"       vec4 b = bGradientTransform * vec4(uv, 0.0f, 1.0f);\n"
"       vec4 borderColor = mix(bx, by, b.y) * max((outerMask - innerMask), 0.0);\n"
"	FragColor = borderColor + mix(innerColor*tint, vec4(fill.xyz, innerColor.a*tint.a), fill.a);\n"
"}\0";

const char* textVertexSource =
"#version 410 core\n"
"layout (location = 0) in vec4 vertex;\n"
"out vec2 texCoords;\n"
"\n"
"uniform mat4 modelMatrix;\n"
"uniform mat4 projectionMatrix;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = projectionMatrix * modelMatrix * vec4(vertex.xy, 0.0, 1.0);\n"
"	texCoords = vertex.zw;\n"
"}\0";

const char* textFragmentSource =
"#version 410 core\n"
"in vec2 texCoords;\n"
"out vec4 FragColor;\n"
"\n"
"uniform sampler2D text;\n"
"uniform vec3 color;\n"
"\n"
"void main()\n"
"{\n"
"	float alpha = texture(text, texCoords).r;\n"
"	FragColor = vec4(color*alpha, alpha);\n"
"}\0";

const char* viewVertexSource =
"#version 410 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 TexCoord;\n"
"out vec2 uv;\n"
"\n"
"void main()\n"
"{\n"
"       gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"       uv = TexCoord;\n"
"}\0";

silver::Shader flatShaded = silver::Shader();
silver::Shader imageShader = silver::Shader();
silver::Shader textShader = silver::Shader();
silver::Shader viewShader = silver::Shader();
