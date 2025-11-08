#include <vector>
#include "../include/silver.hpp"

const char* crtShader =
"#version 460 core\n"
"in vec2 uv;\n"
"uniform float iTime;\n"
"uniform sampler2D iChannel0;\n"
"out vec4 fragColor;\n"
"\n"
"float random (vec2 st) {\n"
"	return fract(sin(dot(st.xy, vec2(12.9898,78.233)))*43758.5453123+iTime);\n"
"}\n"
"vec3 gaussianBlur(sampler2D tex, vec2 uv, vec2 texelSize, int radius, float sigma) {\n"
"    vec3 color = vec3(0.0);\n"
"    float totalWeight = 0.0;\n"
"\n"
"    for (int xi = -radius; xi <= radius; xi++) {\n"
"        for (int yi = -radius; yi <= radius; yi++) {\n"
"            float x = float(xi);\n"
"            float y = float(yi);\n"
"\n"
"            vec2 offset = vec2(x, y) * texelSize;\n"
"            float weight = exp(-(x * x + y * y) / (2.0 * sigma * sigma));\n"
"            color += texture(tex, uv + offset).rgb * weight;\n"
"            totalWeight += weight;\n"
"        }\n"
"    }\n"
"\n"
"    return color / totalWeight;\n"
"}\n"
"void main()\n"
"{\n"
"	vec2 distorted = uv * 2.0 - 1.0;\n"
"	vec2 pixelSize = 1.0/vec2(1920.0, 1080.0);\n"
"	float r = length(distorted);\n"
"	float s = 20.0;\n"
"	distorted = distorted * (1.0 + s + r * r)/18.5;\n"
"	distorted += 1.0;\n"
"	distorted /= 2.0;\n"
"	if (distorted.x>1.0 || distorted.x<0.0 || distorted.y>1.0 || distorted.y<0.0)\n"
"	{\n"
"		fragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
"		return;\n"
"	};\n"
"	vec4 image = texture(iChannel0, distorted);\n"
"	image.xyz -= clamp(sin(distorted.y*600.0+iTime*50.0), 0.0, 1.0)*0.1*smoothstep(0.75, 0.0, r)*1.5;\n"
"	image += (random(distorted)*0.2)*smoothstep(1.0, 0.0, r)*2.0;\n"
"	image.r *= 0.0;\n"
"	image.g *= 1.4;\n"
"	image.b *= 0.5;\n"
"	image *= 0.5;\n"
"	float l = image.r + image.g + image.b;\n"
"	if (l>0.8)\n"
"	{\n"
"	        image *= 1.8;\n"
"	} else {\n"
"		image *= 0.8;\n"
"	};\n"
"	vec4 glow = vec4(gaussianBlur(iChannel0, distorted, pixelSize, 10, 50.0), 1.0);\n"
"       glow.r *= 0.0;\n"
"       glow.g *= 1.4;\n"
"       glow.b *= 0.5;\n"
"       glow *= 0.5;\n"
"	float lg = glow.r + glow.g + glow.b;\n"
"       if (lg>0.2)\n"
"       {\n"
"               glow *= 1.0;\n"
"       } else {\n"
"               glow *= 0.0;\n"
"       };\n"
"	image += glow*0.75;\n"
"	fragColor = vec4(image.rgb, 1.0);\n"
"}\n";

silver::Shader crt;
silver::Font terminal;

struct contentView : silver::View
{
	silver::Text* t;
	silver::Quad* q;
	silver::Image *i;

	contentView()
	{
		terminal = silver::Font("./assets/terminal.ttf");

		crt = silver::Shader(viewVertexSource, crtShader);
		this->shader = &crt;

		background(silver::hex(0x1c1c1c));
		t = new silver::Text("Welcome to RobCo Termex v1.3.2;");
		t->fill(silver::hex(0xffffff))
		 ->font(terminal)
		 ->position(-0.4, 0.4, 0.0);
	};

	std::vector<silver::View*> body() override
	{
		std::vector<silver::View*> e = {
			t
		};

		return e;
	};
};

int main()
{
	silver::App app(640, 480, "windowTest");
	app.mainView(new contentView())
	   .run();
	return 0;
};
