#include "./Lib.hpp"
#include "./State.hpp"

silver::State::State(float value)
{
	x = value;
	silver::states.push_back(this);
	index = silver::states.size() - 1;
	animation = {1, 0.0, 0.0, 0.0, var, 1.0, linear};
};

silver::State &silver::State::operator=(float value)
{
	animation.finished = 0;
	animation.progress = 0.0;
	animation.initial = x;
	animation.target = value;
	animation.var = var;
	x = value;
	return *this;
};

silver::State &silver::State::operator=(double value)
{
        animation.finished = 0;
        animation.progress = 0.0;
        animation.initial = x;
        animation.target = static_cast<float>(value);
        animation.var = var;
        x = static_cast<float>(value);
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
