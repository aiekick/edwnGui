#pragma once
#include "../../EGui.hh"

struct Anims {
	EGUI_API float lerp(float x, float goal, float time);
	EGUI_API float linear(float t, float b, float c, float d);
};

extern Anims Animations;