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

float easeOutElastic(float t)
{
	float c4 = (2.0 * M_PI) / 3.0;
	return t == 0.0 ? 0.0 : t == 1.0 ? 1.0 : std::pow(2.0, -10.0 * t) * std::sin((t * 10.0 - 0.75) * c4) + 1.0;
};

float easeOutBack(float t)
{
	float c1 = 1.70158;
	float c3 = c1 + 1.0;

	return 1.0 + c3 * std::pow(t - 1.0, 3.0) + c1 * std::pow(t - 1.0, 2.0);
};

int animate(float time, float(*easing)(float), int state)
{
	silver::states[state]->animation.time = time;
        silver::states[state]->animation.easing = easing;
	return state;
};

int animate(float time, float delay, float(*easing)(float), int state)
{
        silver::states[state]->animation.time = time;
        silver::states[state]->animation.easing = easing;
        silver::states[state]->animation.delay = delay;
        return state;
};

void silver::Animation::animate()
{
	if (progress>=1.0)
	{
		*var = target;
		finished = 1;
	} else {
		if (dtime>0.0)
		{
			dtime -= 1.0/60.0;
		} else {
			float x = easing(progress);
			*var = initial + (x*(target-initial));
			progress += 1/(time*60.0);
		};
	};
};
