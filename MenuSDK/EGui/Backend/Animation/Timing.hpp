#pragma once
#include "../../EGui.hpp"

struct Timing {
	int FrameRate_Max = 0;

	float getFrameTime();
	float getDeltaTime();
	int getFrameRate();
	int getFrameRateMax();
	void updateDeltaTime();
};

extern Timing timing;