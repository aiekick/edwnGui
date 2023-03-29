#include "Timing.hpp"
Timing timing;

using namespace std::chrono;

float Timing::getFrameTime() {
    static double frequency = 0.0;
    static LARGE_INTEGER prevTime;
    static bool initialized = false;

    if (!initialized) {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        frequency = static_cast<double>(freq.QuadPart);
        QueryPerformanceCounter(&prevTime);
        initialized = true;
    }

    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    double elapsedTime = static_cast<double>(currentTime.QuadPart - prevTime.QuadPart) / frequency;
    prevTime = currentTime;

    return static_cast<float>(elapsedTime);
}

float Timing::getRealTime() {
    auto current_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double>>(current_time.time_since_epoch()).count();
}

static auto lastTime = high_resolution_clock::now();
static float deltaTime = 0.f;

void Timing::updateDeltaTime() {
    auto currentTime = high_resolution_clock::now();
    double thisDeltaTime = duration_cast<duration<double>>(currentTime - lastTime).count();

    lastTime = currentTime;
    deltaTime = thisDeltaTime;

    return;
}

void Timing::updateFrameRateAbs() {
    static int FrameCount = 0;
    static auto startTime = std::chrono::high_resolution_clock::now();

    static float FramesPerSecond = 0.0f;
    FrameCount++;

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

    if (elapsedTime >= 1000.0f) {
        FramesPerSecond = (float)FrameCount / (elapsedTime / 1000.0f);
        FrameCount = 0;
        startTime = currentTime;
    }

    FrameRate_Abs = FramesPerSecond;

    if (FrameRate_Abs > FrameRate_Abs_Max)
        FrameRate_Abs_Max = FrameRate_Abs;
}

int Timing::getFrameRateAbs() {
    return FrameRate_Abs;
}

int Timing::getFrameRateAbsMax() {
    return FrameRate_Abs_Max;
}

float Timing::getDeltaTime() {
    return deltaTime;
}

int Timing::getFrameRate() {
    int fps = 1.f / getDeltaTime();

    if (fps > FrameRate_Max)
        FrameRate_Max = fps;

    return fps;
}

int Timing::getFrameRateMax() {
    return FrameRate_Max;
}