#pragma once
#include "../../EGui.hh"

struct Timing {
	EGUI_API float getDeltaTime();
};

extern Timing timing;