#pragma once
#include "../../EGui.hpp"

struct Anims {
	float lerp(float x, float goal, float time) {
		return float(x + (goal - x) * time);
	}

	float linear(float in, float t, float c, float d) {
		return c * t / d + in;
	}

	float Flip(float x) {
		return 1 - x;
	}

	float Square(float value)
	{
		float OddNum = 1.0;
		float SquareNum = 0.0;

		value = abs(value);

		while (value--)
		{
			SquareNum = SquareNum + OddNum;
			OddNum = OddNum + 2;
		}

		return SquareNum;
	}

	float EaseIn(float t)
	{
		return t * t;
	}

	float EaseOut(float t)
	{
		return Flip(Square(Flip(t)));
	}

	float EaseInOut(float t) {
		return lerp(EaseIn(t), EaseOut(t), t);
	}

	float Spike(float t)
	{
		if (t <= .5f)
			return EaseIn(t / .5f);

		return EaseIn(Flip (t) / .5f);
	}
};

extern Anims Animations;