#include "./Lib.hpp"
#include "./State.hpp"

silver::State::State(float value)
{
	x = value;
	silver::states.push_back(this);
	index = silver::states.size() - 1;
	animation = {1, 0.0, 0.0, 0.0, var, 1.0, 0.0, 0.0, linear};
};

void silver::State::dispatchAnimation(float value)
{
        animation.finished = 0;
        animation.progress = 0.0;
        animation.initial = x;
        animation.target = value;
        animation.var = var;
	animation.dtime = animation.delay;
};

silver::State &silver::State::operator=(float value)
{
	dispatchAnimation(value);
	x = value;
	return *this;
};

silver::State &silver::State::operator=(double value)
{
	dispatchAnimation(static_cast<float>(value));
        x = static_cast<float>(value);
        return *this;
};

silver::State &silver::State::operator+=(float value)
{
	dispatchAnimation(x + value);
        x = x + value;
        return *this;
};

silver::State &silver::State::operator+=(double value)
{
	dispatchAnimation(x + static_cast<float>(value));
        x = x + static_cast<float>(value);
        return *this;
};

bool silver::State::operator==(float value) const
{
	return (x==value);
};

bool silver::State::operator==(double value) const
{
        return (x==static_cast<float>(value));
};

silver::State::operator int() const {
	return index;
};

silver::State::operator float() const {
        return x;
};

void silver::State::set(float value)
{
	x = value;
};

