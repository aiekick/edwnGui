#include "Timing.hh"
Timing timing;

EGUI_API float Timing::getDeltaTime() {
	return utility.GetFPS() / 1000.f;
}