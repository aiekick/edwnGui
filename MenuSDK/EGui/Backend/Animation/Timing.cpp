#include "Timing.hpp"
Timing timing;

using namespace std::chrono;

float Timing::getFrameTime() {
    static double frequency = 0.0;
    static LARGE_INTEGER prevTime;
    static bool initialized = false;

    // Get the frequency of the high-resolution performance counter
    if (!initialized)
    {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        frequency = static_cast<double>(freq.QuadPart);
        QueryPerformanceCounter(&prevTime);
        initialized = true;
    }

    // Get the current time
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    // Calculate the elapsed time in seconds
    double elapsedTime = static_cast<double>(currentTime.QuadPart - prevTime.QuadPart) / frequency;

    // Update the previous time
    prevTime = currentTime;

    // Return the delta time as a float
    return static_cast<float>(elapsedTime);
}

// Define a global variable to store the last time the function was called
static auto lastTime = high_resolution_clock::now();
static float deltaTime = 0.f;

void Timing::updateDeltaTime() {
    // Get the current time
    auto currentTime = high_resolution_clock::now();

    // Calculate the time elapsed since the last time the function was called
    double thisDeltaTime = duration_cast<duration<double>>(currentTime - lastTime).count();

    // Update the last time the function was called
    lastTime = currentTime;

    //Update the delta time.
    deltaTime = thisDeltaTime;

    return;
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