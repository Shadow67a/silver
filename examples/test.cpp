#include <vector>

#include "../include/silver.hpp"

const char* backgroundShaderSource =
"#version 460 core\n"
"in vec2 uv;\n"
"uniform float iTime;\n"
"uniform sampler2D iChannel0;\n"
"out vec4 fragColor;\n"
"\n"
"float hash(float x) {\n"
"    return fract(sin(x) * 43758.5453123);\n"
"}\n"
"\n"
"float noise1D(float x) {\n"
"    float i = floor(x);\n"
"    float f = fract(x);\n"
"    float a = hash(i);\n"
"    float b = hash(i + 1.0);\n"
"    float t = f*f*(3.0 - 2.0*f);\n"
"    return mix(a, b, t);\n"
"}\n"
"\n"
"float fbm1D(float x) {\n"
"    float total = 0.0;\n"
"    float amp   = 0.5;\n"
"    float freq  = 1.0;\n"
"    \n"
"    for (int i = 0; i < 5; i++) {\n"
"        total += noise1D(x * freq) * amp;\n"
"        freq  *= 2.0;\n"
"        amp   *= 0.5;\n"
"    }\n"
"    return total;\n"
"}\n"
"\n"
"vec3 wave(vec2 uv, float thickness, float yoffset, float frequency, float amplitude)\n"
"{\n"
"    float x = uv.x*frequency+iTime;\n"
"    float y = sin(x)*amplitude+yoffset+fbm1D(x*0.1)*0.06;\n"
"    float d = abs(uv.y-y);\n"
"    float line = 1.0;\n"
"    if (uv.y<y && d<=thickness)\n"
"    {\n"
"        line = smoothstep(thickness*10.0/2.0, 0.0, d)*0.3;\n"
"        line = pow(line, 1.5);\n"
"        line*=sin(x*2.0)+1.5;\n"
"        line*=0.7;\n"
"    } else {\n"
"        line = smoothstep(0.005, 0.0, d)*0.35;\n"
"        line = pow(line, 1.5);\n"
"    };\n"
"    vec3 color = vec3(1.0, 1.0, 1.0);\n"
"    return color*line;\n"
"}\n"
"\n"
"void main()\n"
"{\n"
"   \n"
"    vec4 background = vec4(0.008, 0.49, 0.039, 1.0);\n"
"    float gradient = smoothstep(1.0, 0.0, uv.y)*1.0;\n"
"    vec3 line1 = wave(uv, sin(uv.x*3.0+iTime*1.0)*0.03, 0.5, 5.0, 0.1);\n"
"    vec3 line2 = wave(uv, sin(uv.x*2.5+iTime*1.0+3.0)*0.03, 0.5, 5.5, 0.15);\n"
"    vec3 line3 = wave(uv, sin(uv.x*2.7+iTime*1.0+5.7)*0.03, 0.48, 4.3, 0.13);\n"
"    vec3 line4 = wave(uv, sin(uv.x*2.2+iTime*1.0+5.7)*0.02, 0.46, 3.3, 0.07);\n"
"    vec3 final = mix(mix(mix(mix(background.xyz*gradient, line1, 0.5)*2.0, line2, 0.5)*2.0, line3, 0.5)*2.0, line4, 0.5)*2.0;\n"
"    fragColor = vec4(final, 1.0);\n"
"}\n";

silver::Shader backgroundShader;

struct contentView: silver::View
{
	contentView()
	{
		backgroundShader = silver::Shader(viewVertexSource, backgroundShaderSource);
		this->shader = &backgroundShader;
	};

	std::vector<silver::View*> body() override
        {
                std::vector<silver::View*> e = {
                };

                return e;
        };
};

int main()
{
        silver::App app(640, 480, "test");
        app.mainView(new contentView())
           .run();
        return 0;
};
