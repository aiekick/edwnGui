#include "Animation.h"

Anims Animations;

EGUI_API float Anims::lerp(float x, float goal, float time) {
	return x + (goal - x) * time;
}

EGUI_API float Anims::linear(float t, float b, float c, float d) {
	return c * t / d + b;
}