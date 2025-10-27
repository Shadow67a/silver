#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "./Lib.hpp"

class silver::Animation
{
	public:
		bool finished;
		float progress;
		float initial;
		float target;
		float *var;
		float time;
		float (*easing)(float);
		void animate();
};

int animate(float time, float(*easing)(float), int state);

float easeOutExpo(float t);
float linear(float t);

#endif
