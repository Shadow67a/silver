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
"#version 460 core\n"
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
"};\0";

const char* flatShadedFragmentSource =
"#version 460 core\n"
"out vec4 FragColor;\n"
"in vec2 uv;\n"
"uniform vec4 x;\n"
"uniform vec4 y;\n"
"uniform mat4 gradientTransform;\n"
"uniform vec4 bx;\n"
"uniform vec4 by;\n"
"uniform mat4 bGradientTransform;\n"
"uniform float borderWidth;"
"uniform float cornerRadius;"
"\n"
"void main()\n"
"{\n"
"    vec4 v = gradientTransform * vec4(uv, 0.0f, 1.0f);\n"
"    vec4 final = mix(x, y, v.y);\n"
"    vec4 b = bGradientTransform * vec4(uv, 0.0f, 1.0f);\n"
"    vec4 border = mix(bx, by, b.y);\n"
"    float bw = borderWidth*0.01;\n"
"    float iradius = cornerRadius-bw;\n"
"    vec2 itr = vec2(1.0-iradius-bw, 1.0-iradius-bw);\n"
"    vec2 itl = vec2(iradius+bw, 1.0-iradius-bw);\n"
"    vec2 ibr = vec2(1.0-iradius-bw, iradius+bw);\n"
"    vec2 ibl = vec2(iradius+bw, iradius+bw);\n"
"    if (uv.x > itr.x && uv.y > itr.y)\n"
"    {\n"
"        float d = distance(uv, itr);\n"
"        if (d>iradius)\n"
"        {\n"
"            final = border;\n"
"        };\n"
"    };\n"
"    if (uv.x < itl.x && uv.y > itl.y)\n"
"    {\n"
"        float d = distance(uv, itl);\n"
"        if (d>iradius)\n"
"        {\n"
"            final = border;\n"
"        };\n"
"    };\n"
"    if (uv.x > ibr.x && uv.y < ibr.y)\n"
"    {\n"
"        float d = distance(uv, ibr);\n"
"        if (d>iradius)\n"
"        {\n"
"            final = border;\n"
"        };\n"
"    };\n"
"    if (uv.x < ibl.x && uv.y < ibl.y)\n"
"    {\n"
"        float d = distance(uv, ibl);\n"
"        if (d>iradius)\n"
"        {\n"
"            final = border;\n"
"        };\n"
"    };\n"
"    if ( (uv.x>=(0.0)&&uv.x<=(0.0+bw)) || (uv.x>=(1.0-bw)&&uv.x<=(1.0)) || (uv.y>=(0.0)&&uv.y<=(0.0+bw)) || (uv.y>=(1.0-bw)&&uv.y<=(1.0)) )\n"
"    {\n"
"        final = border;\n"
"    };\n"
"    vec2 tr = vec2(1.0-cornerRadius, 1.0-cornerRadius);\n"
"    vec2 tl = vec2(cornerRadius, 1.0-cornerRadius);\n"
"    vec2 br = vec2(1.0-cornerRadius, cornerRadius);\n"
"    vec2 bl = vec2(cornerRadius, cornerRadius);\n"
"    if (uv.x > tr.x && uv.y > tr.y)\n"
"    {\n"
"        float d = distance(uv, tr);\n"
"        if (d>cornerRadius)\n"
"        {\n"
"            discard;\n"
"        };\n"
"    };\n"
"    if (uv.x < tl.x && uv.y > tl.y)\n"
"    {\n"
"        float d = distance(uv, tl);\n"
"        if (d>cornerRadius)\n"
"        {\n"
"            discard;\n"
"        };\n"
"    };\n"
"    if (uv.x > br.x && uv.y < br.y)\n"
"    {\n"
"        float d = distance(uv, br);\n"
"        if (d>cornerRadius)\n"
"        {\n"
"        discard;\n"
"        };\n"
"    };\n"
"    if (uv.x < bl.x && uv.y < bl.y)\n"
"    {\n"
"        float d = distance(uv, bl);\n"
"        if (d>cornerRadius)\n"
"        {\n"
"            discard;\n"
"        };\n"
"    };\n"
"    FragColor = final;\n"
"};\0";

const char* imageVertexSource =
"#version 460 core\n"
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
"};\0";

const char* imageFragmentSource =
"#version 460 core\n"
"out vec4 FragColor;\n"
"in vec2 uv;\n"
"uniform sampler2D image;\n"
"uniform vec4 bx;\n"
"uniform vec4 by;\n"
"uniform mat4 bGradientTransform;\n"
"uniform float borderWidth;"
"uniform float cornerRadius;"
"\n"
"void main()\n"
"{\n"
"       vec4 final = texture(image, uv);\n"
"       vec4 b = bGradientTransform * vec4(uv, 0.0f, 1.0f);\n"
"       vec4 border = mix(bx, by, b.y);\n"
"       float bw = borderWidth*0.01;\n"
"    float iradius = cornerRadius-bw;\n"
"    vec2 itr = vec2(1.0-iradius-bw, 1.0-iradius-bw);\n"
"    vec2 itl = vec2(iradius+bw, 1.0-iradius-bw);\n"
"    vec2 ibr = vec2(1.0-iradius-bw, iradius+bw);\n"
"    vec2 ibl = vec2(iradius+bw, iradius+bw);\n"
"    if (uv.x > itr.x && uv.y > itr.y)\n"
"    {\n"
"        float d = distance(uv, itr);\n"
"        if (d>iradius)\n"
"        {\n"
"            final = border;\n"
"        };\n"
"    };\n"
"    if (uv.x < itl.x && uv.y > itl.y)\n"
"    {\n"
"        float d = distance(uv, itl);\n"
"        if (d>iradius)\n"
"        {\n"
"            final = border;\n"
"        };\n"
"    };\n"
"    if (uv.x > ibr.x && uv.y < ibr.y)\n"
"    {\n"
"        float d = distance(uv, ibr);\n"
"        if (d>iradius)\n"
"        {\n"
"            final = border;\n"
"        };\n"
"    };\n"
"    if (uv.x < ibl.x && uv.y < ibl.y)\n"
"    {\n"
"        float d = distance(uv, ibl);\n"
"        if (d>iradius)\n"
"        {\n"
"            final = border;\n"
"        };\n"
"    };\n"
"       if ( (uv.x>=(0.0)&&uv.x<=(0.0+bw)) || (uv.x>=(1.0-bw)&&uv.x<=(1.0)) || (uv.y>=(0.0)&&uv.y<=(0.0+bw)) || (uv.y>=(1.0-bw)&&uv.y<=(1.0)) )\n"
"       {\n"
"           final = border;\n"
"       };\n"
"    vec2 tr = vec2(1.0-cornerRadius, 1.0-cornerRadius);\n"
"    vec2 tl = vec2(cornerRadius, 1.0-cornerRadius);\n"
"    vec2 br = vec2(1.0-cornerRadius, cornerRadius);\n"
"    vec2 bl = vec2(cornerRadius, cornerRadius);\n"
"    if (uv.x > tr.x && uv.y > tr.y)\n"
"    {\n"
"        float d = distance(uv, tr);\n"
"        if (d>cornerRadius)\n"
"        {\n"
"            discard;\n"
"        };\n"
"    };\n"
"    if (uv.x < tl.x && uv.y > tl.y)\n"
"    {\n"
"        float d = distance(uv, tl);\n"
"        if (d>cornerRadius)\n"
"        {\n"
"            discard;\n"
"        };\n"
"    };\n"
"    if (uv.x > br.x && uv.y < br.y)\n"
"    {\n"
"        float d = distance(uv, br);\n"
"        if (d>cornerRadius)\n"
"        {\n"
"        discard;\n"
"        };\n"
"    };\n"
"    if (uv.x < bl.x && uv.y < bl.y)\n"
"    {\n"
"        float d = distance(uv, bl);\n"
"        if (d>cornerRadius)\n"
"        {\n"
"            discard;\n"
"        };\n"
"    };\n"
"	FragColor = final;\n"
"};\0";

const char* textVertexSource =
"#version 460 core\n"
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
"};\0";

const char* textFragmentSource =
"#version 460 core\n"
"in vec2 texCoords;\n"
"out vec4 FragColor;\n"
"\n"
"uniform sampler2D text;\n"
"uniform vec3 color;\n"
"\n"
"void main()\n"
"{\n"
"	float alpha = texture(text, texCoords).r;\n"
"	FragColor = vec4(color, alpha);\n"
"};\0";

const char* viewVertexSource =
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 TexCoord;\n"
"out vec2 uv;\n"
"\n"
"void main()\n"
"{\n"
"       gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"       uv = TexCoord;\n"
"};\0";

silver::Shader flatShaded = silver::Shader();
silver::Shader imageShader = silver::Shader();
silver::Shader textShader = silver::Shader();
silver::Shader viewShader = silver::Shader();
