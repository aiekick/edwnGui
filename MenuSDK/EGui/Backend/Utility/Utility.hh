#pragma once
#include "../../EGui.hh"

struct Util {
	EGUI_API float GetFPS();
	EGUI_API string GetFPS_str();
	EGUI_API long map(long x, long in_min, long in_max, long out_min, long out_max);
};

extern Util utility;