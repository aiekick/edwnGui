#pragma once
#include "../../EGui.hpp"

struct Timing {
	float getFrameTime();
	float getDeltaTime();
	int getFrameRate();
	void updateFrameRate();
	void updateDeltaTime();
};

extern Timing timing;