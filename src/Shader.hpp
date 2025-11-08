#ifndef SHADER_HPP
#define SHADER_HPP

#include "./glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Lib.hpp"

class silver::Shader
{
	private:
		unsigned int program;
		unsigned int vertexShader;
		unsigned int fragmentShader;
	public:
		Shader();
		Shader(const char* vertexShaderSource, const char* fragmentShaderSource);

		void setInt(const char* uniform, int &value);
		void setFloat(const char* uniform, float &value);
		void setVec3(const char* uniform, glm::vec3 &value);
		void setVec4(const char* uniform, glm::vec4 &value);
		void setMat4(const char* uniform, glm::mat4 &value);
		void use();
};

extern const char* flatShadedVertexSource;
extern const char* flatShadedFragmentSource;

extern const char* imageVertexSource;
extern const char* imageFragmentSource;

extern const char* textVertexSource;
extern const char* textFragmentSource;

extern const char* viewVertexSource;
extern const char* viewFragmentSource;

extern silver::Shader flatShaded;
extern silver::Shader imageShader;
extern silver::Shader textShader;
extern silver::Shader viewShader;

#endif
