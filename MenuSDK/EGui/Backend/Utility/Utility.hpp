#pragma once
#include "../../EGui.hpp"

struct Util {
	void UpdateFpsCounter();
	int GetFps();
	long map(long x, long in_min, long in_max, long out_min, long out_max);
};

extern Util utility;