#pragma once
#include "../../EGui.hpp"

struct Timing {
	int FrameRate_Max = 0;
	int FrameRate_Abs = 0;
	int FrameRate_Abs_Max = 0;

	float getFrameTime();
	float getDeltaTime();
	int getFrameRate();
	int getFrameRateMax();
	void updateFrameRateAbs();
	int getFrameRateAbs();
	int getFrameRateAbsMax();
	void updateDeltaTime();
	float getRealTime();
};

extern Timing timing;