#ifndef STATE_HPP
#define STATE_HPP

#include "./Lib.hpp"
#include "./Animation.hpp"

class silver::State
{
	public:
		float x;
		float *var;
		int index;
		silver::Animation animation;

		State (float value);

		State &operator=(float);
		State &operator=(double);
		bool operator==(float) const;
		bool operator==(double) const;
		operator int() const;
		operator float() const;
};

#endif

