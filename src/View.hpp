#ifndef VIEW_HPP
#define VIEW_HPP

#include <vector>
#include <functional>
#include <variant>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Lib.hpp"
#include "./Shader.hpp"
#include "./Animation.hpp"
#include "./State.hpp"

class silver::View
{
	public:
		Shader* shader;
		unsigned int framebuffer;
		unsigned int interFrameBuffer;
		unsigned int renderTexture;
		unsigned int screenTexture;
		unsigned int rbo;
		unsigned int VBO;
		unsigned int VAO;

		glm::mat4 modelMatrix;
		glm::vec3 _position;
                glm::vec3 _rotation;
                glm::vec3 _scale;

		glm::vec3 backgroundColor;

		glm::vec4 x;
                glm::vec4 y;
                glm::mat4 gradientTransform;

		glm::vec4 bx;
                glm::vec4 by;
                glm::mat4 bGradientTransform;
		float borderWidth;
		float _cornerRadius;

		std::function<void()> _onHover;
		std::function<void()> _onLeave;
		std::function<void()> _onClick;
		bool _hovered;

		View* _parent;

		View();

		void resize();
		virtual View* onClick(std::function<void()> callback);
		virtual View *background(glm::vec3 color);
		virtual View *border(std::variant<Gradient, glm::vec4>, std::variant<float, double, int>);
		virtual View *border(std::variant<Gradient, glm::vec4>);
		virtual View *cornerRadius(std::variant<float, double, int> v);

		virtual View *fill(std::variant<glm::vec4, Gradient> color);

		virtual View *position(std::variant<float, double, int> x, std::variant<float, double, int> y, std::variant<float, double, int> z);
		virtual View *rotation(float x, float y, float z);
		virtual View *scale(float x, float y);

		virtual View* onHover(std::function<void()> callback);
		virtual View* onHover(std::function<void()> callback, std::function<void()> onLeave);
		virtual void hoverHandler();

		virtual View* parent(View* parentView);

		virtual std::vector<silver::View*> body();
		virtual void render();
};

#endif
