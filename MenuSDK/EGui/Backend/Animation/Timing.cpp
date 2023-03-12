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

static int FrameRate = 0;
void Timing::updateFrameRate() {
    static int frameCount = 0;
    static auto startTime = std::chrono::high_resolution_clock::now();
    static float fps_ = 0.0f;
    frameCount++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    if (elapsedTime >= 1000.0f) {
        fps_ = (float)frameCount / (elapsedTime / 1000.0f);
        frameCount = 0;
        startTime = currentTime;
    }

    FrameRate = fps_;
}

int Timing::getFrameRate() {
    return FrameRate;
}