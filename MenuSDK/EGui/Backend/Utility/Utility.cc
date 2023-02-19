#include "Utility.hh"
#include <chrono>

Util utility;

EGUI_API float Util::GetFPS() {
    static int frameCount = 0;
    static auto startTime = std::chrono::high_resolution_clock::now();
    static float fps = 0.0f;
    frameCount++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    if (elapsedTime >= 1000.0f) {
        fps = (float)frameCount / (elapsedTime / 1000.0f);
        frameCount = 0;
        startTime = currentTime;
    }
    return fps;
}

EGUI_API string Util::GetFPS_str() {
    return std::to_string(GetFPS());
}

EGUI_API long Util::map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}