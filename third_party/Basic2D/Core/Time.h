#pragma once

#include <SDL3/SDL.h>

struct TimeData
{
    Uint64 lastCounter = 0;
    Uint64 frequency = 0;
    float deltaTime = 0.0f;
    float totalTime = 0.0f;
    bool initialized = false;
};

static TimeData timeData;

static void UpdateTime()
{
    if (!timeData.initialized)
    {
        timeData.frequency = SDL_GetPerformanceFrequency();
        timeData.lastCounter = SDL_GetPerformanceCounter();
        timeData.initialized = true;
    }

    Uint64 currentCounter = SDL_GetPerformanceCounter();
    Uint64 deltaCounter = currentCounter - timeData.lastCounter;
    timeData.lastCounter = currentCounter;

    timeData.deltaTime = (float)((double)deltaCounter / (double)timeData.frequency);

    const float maxDeltaTime = 0.25f;
    if (timeData.deltaTime > maxDeltaTime)
        timeData.deltaTime = maxDeltaTime;

    timeData.totalTime += timeData.deltaTime;
}

static float GetDeltaTime()
{
    return timeData.deltaTime;
}

static float GetTotalTime()
{
    return timeData.totalTime;
}