#include <cmath>
#include <string>
#include <iostream>
#include "../include/silver.hpp"

#define NUM 37
#define DIST 0.03
#define SHELL 6.0
#define FRONT 0.82
#define SIZE 0.03
#define THRESHOLD 0.07
#define ATTN 0.1
#define LIMIT (SIZE+THRESHOLD)/(1.0+ATTN)
#define OVER 0.07
#define BASE_DELAY 5.0
#define DELAY 0.01

float rad(float deg)
{
	return (deg*M_PI)/180.0;
};

float rubberband(float d, float size)
{
	return (d) / (1.0 + size * std::abs(d));
}

struct Icon
{
	silver::Image* image;
	float r;
	float theta;
	silver::State z{1.0};
};

struct contentView : silver::View
{
	Icon icons[NUM];
	silver::Quad* box;
	silver::State bx{0.0};
	silver::State by{0.0};
	bool outX;
	bool outY;
	bool top;
	bool right;
	float shell = 1;
	float shellIcon = 1;
	contentView()
	{
		outX = 0;
                outY = 0;
		top = 0;
		right = 0;
		box = new silver::Quad();
		box->scale(1.5, 1.5)
		 ->position(animate(1.0, easeOutExpo, bx), animate(1.0, easeOutExpo, by), 0.0)
		 ->fill(silver::hex(0x000000))
		 ->onDrag([this](float x, float y){
			bx.animation.finished = 1;
			by.animation.finished = 1;
                        float ox = x;
			float oy = y;
			if (x>OVER)
			{
				float d = x-OVER;
				float k = OVER+rubberband(d, 8.0);
				ox = k;
				outX = 1;
				right = 1;
			} else if (x<(-OVER))
			{
				float d = x+OVER;
				float k = -OVER+rubberband(d, 8.0);
                                ox = k;
				outX = 1;
				right = 0;
			} else {
				outX = 0;
			};
			if (y>OVER)
                        {
                                float d = y-OVER;
                                float k = OVER+rubberband(d, 8.0);
                                oy = k;
				outY = 1;
				top = 1;
                        } else if (y<(-OVER))
                        {
                                float d = y+OVER;
                                float k = -OVER+rubberband(d, 8.0);
                                oy = k;
				outY = 1;
				top = 0;
                        } else {
				outY = 0;
			};
			box->position(ox, oy, 0.0);
		}, [this]{
			if (outX)
			{
				bx.x = box->_position.x;
				bx = right ? OVER : -OVER;
				outX = 0;
			};
                        if (outY)
                        {
                                by.x = box->_position.y;
                                by = top ? OVER : -OVER;
				outY = 0;
                        };
		});
		frame([this]{
			for (int i = 0; i < NUM; i++)
                        {
                                float dist = glm::distance(icons[i].image->_position+box->_position, glm::vec3(0.0, 0.0, FRONT));
                                float s = SIZE-(dist*ATTN);
                                if (dist>LIMIT)
                                {
                                        icons[i].image->scale(0.0, 0.0);
                                }
                                else if (dist>THRESHOLD)
                                {
                                        dist-=THRESHOLD;
                                        icons[i].image->scale(s-dist, s-dist);
                                } else {
                                        float newR = (icons[i].r * std::clamp((s+0.005)/SIZE, 0.0, 1.0));
                                        icons[i].image->scale(s, s)
                                         ->position(newR*std::sin(rad(icons[i].theta)), newR*std::cos(rad(icons[i].theta)), FRONT);
                                };
                        };
		});
		for (int i = 0; i < NUM; i++)
		{
			if (i == 0)
			{
				std::string path = "./assets/icons/" + std::to_string(i+1) + ".png";
				icons[i].image = new silver::Image(path.c_str());
				icons[i].image->scale(SIZE, SIZE)
				 ->position(0.0, 0.0, animate(1.0, BASE_DELAY+((float)i)*DELAY, easeOutBack, icons[i].z))
				 ->cornerRadius(0.5)
				 ->parent(box);
				icons[i].z.set(1.0);
				icons[i].z = FRONT;
			} else {
                                float max = shell * SHELL;
				icons[i].theta = shellIcon * 360.0/(max);
				icons[i].r = shell * DIST;
				std::string path = "./assets/icons/" + std::to_string(i) + ".png";
                                icons[i].image = new silver::Image(path.c_str());
                                icons[i].image->scale(SIZE, SIZE)
				 ->position(icons[i].r*std::sin(rad(icons[i].theta)), icons[i].r*std::cos(rad(icons[i].theta)), animate(1.0, BASE_DELAY+((float)i)*DELAY, easeOutBack, icons[i].z))
				 ->cornerRadius(0.5)
				 ->parent(box);
                                icons[i].z.set(1.0);
                                icons[i].z = FRONT;
				if (shellIcon==max)
				{
					shellIcon = 1;
					shell += 1;
				} else {
					shellIcon += 1;
				};
			};
		};
	};
	void body() override
	{
		for (int i = 0; i < NUM; i++)
		{
			elements.push_back(icons[i].image);
		};
                elements.push_back(box);
	};
};

int main()
{
	silver::App app(640, 480, "watch");
	app.mainView(new contentView())
	   .run();
	return 0;
};

