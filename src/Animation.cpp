#include "./Lib.hpp"
#include "./Animation.hpp"
#include "./State.hpp"

#include <cmath>

float easeOutExpo(float t)
{
	return t == 1.0 ? 1.0 : 1.0 - pow(2.0, -10.0 * t);
};

float linear(float t)
{
	return t == 1.0 ? 1.0 : t;
};

int animate(float time, float(*easing)(float), int state)
{
	silver::states[state]->animation.time = time;
        silver::states[state]->animation.easing = easing;

	return state;
};

void silver::Animation::animate()
{
	if (progress>=1.0)
	{
		*var = target;
		finished = 1;
	} else {
		float x = easing(progress);
		*var = initial + (x*(target-initial));
		progress += 1/(time*60.0);
	};
};
