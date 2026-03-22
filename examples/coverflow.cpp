#include <vector>
#include <string>
#include "../include/silver.hpp"
#include <iostream>

struct Card
{
	silver::Image* image;
	silver::State rotation{0.0};
	silver::State position{0.0};
};

struct contentView : silver::View
{
	Card cards[10];
	Card reflections[10];
	silver::Quad* box;
	silver::State s{0.0};
	int current;
	bool animating;
	float dragStart;
	contentView()
	{
		animating = false;
		dragStart = 0.0;
		current = 4;
		box = new silver::Quad();
		box->fill(silver::hex(0x000000, 0.0))
		   ->border(silver::hex(0xff0000), 1.0)
		   ->position(animate(4.0, easeOutExpo, s), 0.0, 0.0)
		   ->scale(3.6, 1.0)
		  ->onDrag([this](float x, float y){
                        s.animation.finished = 1;
			box->position(x, 0.0, 0.0);
			float diff = (x-dragStart)/0.15;
			int d = (int)diff;
			if (diff < 0)
			{
				if (current==9)
                                {
                                        float d = dragStart-x;
                                        float s = dragStart - (d / (1.0 + 4.0 * d));
                                        box->position(s, 0.0, 0.0);
                                        return;
                                };
			} else if (diff > 0)
			{
				if (current==0)
                                {
                                        float d = x-dragStart;
                                        float s = dragStart + (d / (1.0 + 4.0 * d));
                                        box->position(s, 0.0, 0.0);
                                        return;
                                };
			};
			if (d<0)
			{
				dragStart = x;
				cards[current].rotation = 50.0;
                       		cards[current].position = 0.0;
				int tmp = cards[current].image->_layer;
				cards[current].image->layer(0);
				cards[current+1].image->layer(tmp+1);
                    		cards[current+1].rotation = 0.0;
                      		cards[current+1].position = 0.2;
				reflections[current].rotation = 50.0;
                                reflections[current].position = 0.0;
				reflections[current].image->layer(0);
				reflections[current+1].image->layer(tmp+1);
                                reflections[current+1].rotation = 0.0;
                                reflections[current+1].position = 0.2;
				current += 1;
			} else if (d>0)
			{
				dragStart = x;
                                cards[current].rotation = -50.0;
                                cards[current].position = 0.0;
				int tmp = cards[current].image->_layer;
                                cards[current].image->layer(cards[current+1].image->_layer);
                                cards[current-1].image->layer(tmp);
                                cards[current-1].rotation = 0.0;
                                cards[current-1].position = 0.2;
				reflections[current].image->layer(cards[current].image->_layer);
                                reflections[current-1].image->layer(tmp);
				reflections[current].rotation = -50.0;
                                reflections[current].position = 0.0;
                                reflections[current-1].rotation = 0.0;
                                reflections[current-1].position = 0.2;
                                current -= 1;
			};
		  }, [this]{
			s.x = box->_position.x;
			s = (-0.15*current)+0.6;
		  });
		for (int i = 0; i < 10; i++)
		{
                        std::string path = "./assets/" + std::to_string(i+1) + ".png";
                        cards[i].image = new silver::Image(path);
                        reflections[i].image = new silver::Image(path);
			if ((i*0.15)-0.6<0.0)
			{
				cards[i].rotation.set(50.0);
                                reflections[i].rotation.set(50.0);
			} else if ((i*0.15)-0.6>0.0)
			{
                                cards[i].rotation.set(-50.0);
                                reflections[i].rotation.set(-50.0);
                                cards[i].image->layer(-i-1);
                                reflections[i].image->layer(-i-1);
			} else if ((i*0.15)-0.6==0.0)
			{
				cards[i].position.set(0.2);
                                reflections[i].position.set(0.2);
			};
			cards[i].image->position((i*0.15)-0.6, 0.0, animate(1.0, easeOutExpo, cards[i].position))
				     ->rotation(0.0, animate(1.0, easeOutExpo, cards[i].rotation), 0.0)
				     ->scale(0.35, 0.35)
				     ->parent(box);
                        reflections[i].image->position((i*0.15)-0.6, -0.35, animate(1.0, easeOutExpo, reflections[i].position))
                                     ->rotation(180.0, animate(1.0, easeOutExpo, reflections[i].rotation), 0.0)
                                     ->scale(0.35, 0.35)
				     ->tint(silver::hex(0xaaaaaa))
                                     ->parent(box);
		};
	};
	void body() override
	{
		for (int i = 0; i < 10; i++)
		{
			elements.push_back(cards[i].image);
			elements.push_back(reflections[i].image);
		};
		elements.push_back(box);
	};
};

int main()
{
	silver::App app(640, 480, "coverflow");
	app.mainView(new contentView)
	   .run();
	return 0;
};
