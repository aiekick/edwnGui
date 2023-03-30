#pragma once
#include "../../EGui.hpp"

struct Anims {
	float easeOutCubic(float t) {
		t -= 1.0f;
		return t * t * t + 1.0f;
	}

	float smoothLerp(float x, float goal, float time) {
		float t = easeOutCubic(time);
		return x + (goal - x) * t;
	}

	float lerp(float x, float goal, float time) {
		return float(x + (goal - x) * time);
	}

	//gay no work lol
	float linear(float x, float goal, float time) {
		return x + (goal - x) / time;
	}

	float Flip(float x) {
		return -x;
	}

	float Square(float value) {
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

	Color ColorLerp(Color clr_in, Color clr_out, float time) {
		return Color(lerp(clr_in.r(), clr_out.r(), time), lerp(clr_in.g(), clr_out.g(), time), lerp(clr_in.b(), clr_out.b(), time), lerp(clr_in.a(), clr_out.a(), time));
	}
};

extern Anims Animations;