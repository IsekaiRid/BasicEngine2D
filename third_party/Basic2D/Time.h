#pragma once

#include <Basic2D/Engine.h>

struct TimeData
{
    double lastTime = -1.0;
    float deltaTime = 0.0f;
    float totalTime = 0.0f;
};

static TimeData timeData;

static void UpdateTime()
{
    double currentTime = (double)SDL_GetTicks() / 1000.0;

    if (timeData.lastTime < 0.0)
        timeData.lastTime = currentTime;

    timeData.deltaTime = (float)(currentTime - timeData.lastTime);
    timeData.lastTime = currentTime;

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