#pragma once
#include "../../EGui.hpp"

struct Timing {
	float getFrameTime();
	float getDeltaTime();
	void updateDeltaTime();
};

extern Timing timing;