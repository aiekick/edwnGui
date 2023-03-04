#pragma once
#include "../../EGui.hpp"

struct Util {
	float GetFPS();
	string GetFPS_str();
	long map(long x, long in_min, long in_max, long out_min, long out_max);
};

extern Util utility;