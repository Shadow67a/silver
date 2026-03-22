#include <iostream>
#include <vector>
#include <functional>
#include <variant>

#include "./glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Lib.hpp"
#include "./View.hpp"
#include "./Shader.hpp"
#include "./Vertices.hpp"
#include "./Animation.hpp"
#include "./State.hpp"

bool dirty;

silver::View::View()
{
	glGenFramebuffers(1, &framebuffer);
	glGenFramebuffers(1, &interFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, renderTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, silver::windowWidth, silver::windowHeight, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, renderTexture, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, silver::windowWidth, silver::windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//init intermediate single-sample FBO;
	glBindFramebuffer(GL_FRAMEBUFFER, interFrameBuffer);
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, silver::windowWidth, silver::windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(viewVertices), viewVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)) );
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader = &viewShader;
	x = glm::vec4(1.0, 1.0, 1.0, 1.0);
	mainView = 0;
	dirty = false;
	_layer = 0;
};

silver::View *silver::View::onClick(std::function<void()> callback)
{
	this->_onClick = callback;
	return this;
};



silver::View *silver::View::background(glm::vec3 color)
{
	this->backgroundColor = color;
	return this;
};

silver::View *silver::View::border(std::variant<Gradient, glm::vec4> color, std::variant<float, double, int> v)
{
	if (std::holds_alternative<float>(v)) {
		this->borderWidth = std::get<float>(v);
        } else if (std::holds_alternative<double>(v)) {
		this->borderWidth = static_cast<float>(std::get<double>(v));
	} else if (std::holds_alternative<int>(v)){
                int state = std::get<int>(v);
		silver::states[state]->var = &borderWidth;
		borderWidth = silver::states[state]->x;
        };
	if (std::holds_alternative<Gradient>(color))
	{
		Gradient g = std::get<Gradient>(color);
		this->bx = g.x;
	        this->by = g.y;
		bGradientTransform = glm::mat4(1.0f);
	        bGradientTransform = glm::rotate(bGradientTransform, g.angle, glm::vec3(0.0f, 0.0f, 1.0f));
	} else if (std::holds_alternative<glm::vec4>(color))
	{
		glm::vec4 c = std::get<glm::vec4>(color);
		this->bx = c;
	        this->by = c;
		bGradientTransform = glm::mat4(1.0f);
	        bGradientTransform = glm::rotate(bGradientTransform, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	};
	return this;
};

silver::View *silver::View::border(std::variant<Gradient, glm::vec4> color)
{
        if (std::holds_alternative<Gradient>(color))
        {
                Gradient g = std::get<Gradient>(color);
                this->bx = g.x;
                this->by = g.y;
                bGradientTransform = glm::mat4(1.0f);
                bGradientTransform = glm::rotate(bGradientTransform, g.angle, glm::vec3(0.0f, 0.0f, 1.0f));
        } else if (std::holds_alternative<glm::vec4>(color))
        {
                glm::vec4 c = std::get<glm::vec4>(color);
                this->bx = c;
                this->by = c;
                bGradientTransform = glm::mat4(1.0f);
                bGradientTransform = glm::rotate(bGradientTransform, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        };
        return this;
};

silver::View *silver::View::fill(std::variant<glm::vec4, Gradient> color)
{
	if (std::holds_alternative<Gradient>(color))
        {
                Gradient g = std::get<Gradient>(color);
                this->x = g.x;
                this->y = g.y;
                gradientTransform = glm::mat4(1.0f);
                gradientTransform = glm::rotate(gradientTransform, g.angle, glm::vec3(0.0f, 0.0f, 1.0f));
        } else if (std::holds_alternative<glm::vec4>(color))
        {
                glm::vec4 c = std::get<glm::vec4>(color);
                this->x = c;
                this->y = c;
                gradientTransform = glm::mat4(1.0f);
                gradientTransform = glm::rotate(gradientTransform, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        };
        return this;
};

silver::View *silver::View::opacity(float value)
{
	this->x.a = value;
	return this;
};

silver::View *silver::View::tint(glm::vec4 color)
{
	this->x = color;
	return this;
};

silver::View *silver::View::cornerRadius(std::variant<float, double, int> v)
{
	if (std::holds_alternative<float>(v)) {
		this->_cornerRadius = std::get<float>(v);
	} else if (std::holds_alternative<double>(v)) {
		this->_cornerRadius = static_cast<double>(std::get<double>(v));
	} else if (std::holds_alternative<int>(v)){
		int state = std::get<int>(v);
		silver::states[state]->var = &_cornerRadius;
        	_cornerRadius = silver::states[state]->x;
	};
	return this;
};

silver::View *silver::View::position(std::variant<float, double, int> x, std::variant<float, double, int> y, std::variant<float, double, int> z)
{
	if (std::holds_alternative<float>(x)) {
                float xPos = std::get<float>(x);
		this->_position.x = xPos;
        } else if (std::holds_alternative<double>(x)){
                float xPos = static_cast<float>(std::get<double>(x));
		this->_position.x = xPos;
        } else if (std::holds_alternative<int>(x)){
                int state = std::get<int>(x);
                silver::states[state]->var = &this->_position.x;
                this->_position.x = silver::states[state]->x;
        };
	if (std::holds_alternative<float>(y)) {
                float yPos = std::get<float>(y);
                this->_position.y = yPos;
        } else if (std::holds_alternative<double>(y)){
                float yPos = static_cast<float>(std::get<double>(y));
                this->_position.y = yPos;
        } else if (std::holds_alternative<int>(y)){
                int state = std::get<int>(y);
                silver::states[state]->var = &this->_position.y;
                this->_position.y = silver::states[state]->x;
        };
	if (std::holds_alternative<float>(z)) {
                float zPos = std::get<float>(z);
                this->_position.z = zPos;
        } else if (std::holds_alternative<double>(z)){
                float zPos = static_cast<float>(std::get<double>(z));
                this->_position.z = zPos;
        } else if (std::holds_alternative<int>(z)){
                int state = std::get<int>(z);
                silver::states[state]->var = &this->_position.z;
                this->_position.z = silver::states[state]->x;
        };
	dirty = true;
	return this;
};

silver::View *silver::View::rotation(std::variant<float, double, int> x, std::variant<float, double, int> y, std::variant<float, double, int> z)
{
	if (std::holds_alternative<float>(x)) {
                float xRot = std::get<float>(x);
                this->_rotation.x = xRot;
        } else if (std::holds_alternative<double>(x)){
                float xRot = static_cast<float>(std::get<double>(x));
                this->_rotation.x = xRot;
        } else if (std::holds_alternative<int>(x)){
                int state = std::get<int>(x);
                silver::states[state]->var = &this->_rotation.x;
                this->_rotation.x = silver::states[state]->x;
        };
        if (std::holds_alternative<float>(y)) {
                float yRot = std::get<float>(y);
                this->_rotation.y = yRot;
        } else if (std::holds_alternative<double>(y)){
                float yRot = static_cast<float>(std::get<double>(y));
                this->_rotation.y = yRot;
        } else if (std::holds_alternative<int>(y)){
                int state = std::get<int>(y);
                silver::states[state]->var = &this->_rotation.y;
                this->_rotation.y = silver::states[state]->x;
        };
        if (std::holds_alternative<float>(z)) {
                float zRot = std::get<float>(z);
                this->_rotation.z = zRot;
        } else if (std::holds_alternative<double>(z)){
                float zRot = static_cast<float>(std::get<double>(z));
                this->_rotation.z = zRot;
        } else if (std::holds_alternative<int>(z)){
                int state = std::get<int>(z);
                silver::states[state]->var = &this->_rotation.z;
                this->_rotation.z = silver::states[state]->x;
        };
	return this;
};

silver::View *silver::View::scale(std::variant<float, double, int> x, std::variant<float, double, int> y)
{
        if (std::holds_alternative<float>(x)) {
                float xSca = std::get<float>(x);
                this->_scale.x = xSca;
        } else if (std::holds_alternative<double>(x)){
                float xSca = static_cast<float>(std::get<double>(x));
                this->_scale.x = xSca;
        } else if (std::holds_alternative<int>(x)){
                int state = std::get<int>(x);
                silver::states[state]->var = &this->_scale.x;
                this->_scale.x = silver::states[state]->x;
        };
        if (std::holds_alternative<float>(y)) {
                float ySca = std::get<float>(y);
                this->_scale.y = ySca;
        } else if (std::holds_alternative<double>(y)){
                float ySca = static_cast<float>(std::get<double>(y));
                this->_scale.y = ySca;
        } else if (std::holds_alternative<int>(y)){
                int state = std::get<int>(y);
                silver::states[state]->var = &this->_scale.y;
                this->_scale.y = silver::states[state]->x;
        };
	return this;
};

void silver::View::hoverHandler()
{
	glm::vec4 topRight = modelMatrix * glm::vec4(0.5, 0.5, 0.0, 1.0);
	glm::vec4 bottomLeft = modelMatrix * glm::vec4(-0.5, -0.5, 0.0, 1.0);

	if (silver::mouse.cursor.x<topRight.x && silver::mouse.cursor.x>bottomLeft.x && silver::mouse.cursor.y<topRight.y && silver::mouse.cursor.y>bottomLeft.y)
	{
		glm::vec2 c(silver::mouse.cursor.x, silver::mouse.cursor.y);
		c.x = (c.x-bottomLeft.x)/(topRight.x-bottomLeft.x);
                c.y = (c.y-bottomLeft.y)/(topRight.y-bottomLeft.y);
		glm::vec2 tr(1.0-_cornerRadius, 1.0-_cornerRadius);
		glm::vec2 tl(_cornerRadius, 1.0-_cornerRadius);
		glm::vec2 br(1.0-_cornerRadius, _cornerRadius);
		glm::vec2 bl(_cornerRadius, _cornerRadius);
		glm::vec2 itr(1.0, 1.0-_cornerRadius);
		glm::vec2 ibl(0.0, _cornerRadius);
		glm::vec2 tir(1.0-_cornerRadius, 1.0);
		glm::vec2 bil(_cornerRadius, 0.0);

		bool ix = (c.x < tir.x) && (c.x > bil.x) && (c.y < tir.y) && (c.y > bil.y);
		bool iy = (c.x < itr.x) && (c.x > ibl.x) && (c.y < itr.y) && (c.y > ibl.y);
		bool i = ix || iy;
		bool d = (glm::distance(c, tr) < _cornerRadius) || (glm::distance(c, tl) < _cornerRadius) || (glm::distance(c, br) < _cornerRadius) || (glm::distance(c, bl) < _cornerRadius);

		if (d||i)
		{
			if (_onHover && !_hovered)
			{
				_onHover();
				_hovered = 1;
			};
			silver::mouse.hovered = 1;
			if (silver::mouse.clicked && _onClick)
			{
				_onClick();
			};
			if (silver::mouse.hovered && (_onHover || _onClick || _onDrag))
			{
				silver::mouse.cursorShape(HAND_CURSOR);
			};
			if (silver::mouse.dragStart && _onDrag)
			{
				dr.initialCursor = silver::mouse.cursor;
				dr.initialPosition = glm::vec2(_position.x, _position.y);
			};
			if(silver::mouse.dragged && _onDrag)
			{
				glm::vec2 pos = dr.initialPosition + (glm::vec2(silver::mouse.cursor.x, silver::mouse.cursor.y)-dr.initialCursor);
				_onDrag(pos.x, pos.y);
			};
	                if (_onDragLeave && silver::mouse.clicked)
			{
	                        //using silver::clicked cuz its triggered on mouse up;
        	                _onDragLeave();
               	 	};
		} else {
			if (_onLeave && _hovered)
			{
				_onLeave();
			};
			_hovered = 0;
			if (!silver::mouse.hovered)
			{
				silver::mouse.cursorShape(ARROW_CURSOR);
			};
		};
	} else {
		if (_onLeave && _hovered)
		{
                        silver::mouse.cursorShape(ARROW_CURSOR);
			_onLeave();
		};
		_hovered = 0;
		if (!silver::mouse.hovered)
		{
			silver::mouse.cursorShape(ARROW_CURSOR);
		};
	};
};

silver::View* silver::View::onHover(std::function<void()> callback)
{
	this->_onHover = callback;
	return this;
};

silver::View* silver::View::onHover(std::function<void()> callback, std::function<void()> onLeave)
{
        this->_onHover = callback;
	this->_onLeave = onLeave;
	return this;
};

silver::View* silver::View::onDrag(std::function<void(float x, float y)> callback)
{
	this->_onDrag = callback;
	return this;
};

silver::View* silver::View::onDrag(std::function<void(float x, float y)> callback, std::function<void()> onLeave)
{
        this->_onDrag = callback;
	this->_onDragLeave = onLeave;
        return this;
};

silver::View* silver::View::frame(std::function<void()> callback)
{
	this->_frame = callback;
	return this;
};

void silver::View::body()
{
	//must be overriden;
};

silver::View* silver::View::parent(silver::View* parentView)
{
	this->_parent = parentView;
	return this;
};

void silver::View::render()
{
	if (mainView)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
	};

	if (_frame)
	{
		_frame();
	};

	for (int i = 0; i < silver::states.size(); i++)
	{
		if (!silver::states[i]->animation.finished)
		{
			silver::states[i]->animation.animate();
			dirty = true;
		};
	};
	if (dirty)
	{
		order();
	};

	//std::vector<silver::View*> elements = body();
	for (int i=0; i<elements.size(); i++)
	{
		elements[i]->hoverHandler();
		elements[i]->render();
	};

	if (mainView)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, interFrameBuffer);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, silver::windowWidth, silver::windowHeight, 0, 0, silver::windowWidth, silver::windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->use();
		shader->setFloat("iTime", silver::iTime);
		shader->setVec4("tint", x);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTexture);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	};
};

void silver::View::resize()
{
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, renderTexture);
       	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, silver::windowWidth, silver::windowHeight, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, silver::windowWidth, silver::windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, silver::windowWidth, silver::windowHeight);
};

void silver::View::order()
{
	if (elements.size()==0)
	{
		return;
	};
	for (size_t i = 0; i < elements.size()-1; i++)
	{
		for (size_t j = 0; j < elements.size()-i-1; j++)
		{
			if (elements[j]->_layer > elements[j+1]->_layer)
			{
				silver::View* temp =  elements[j+1];
				elements[j+1] = elements[j];
				elements[j] = temp;
			};
		};
	};
};

silver::View* silver::View::layer(int index)
{
	_layer = index;
	dirty = true;
	return this;
};
