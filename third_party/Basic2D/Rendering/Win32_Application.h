#pragma once

#include <Basic2D/Engine.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")


float GetProcessCPUUsagePercent();
#endif

struct RenderWindows
{
    glm::vec2 ScreenSize;
};

static RenderWindows renderWindows;
static SDL_Window *sdlWindow = nullptr;
static SDL_GLContext glContext = nullptr;

static bool g_UseDwmFlush = false;

SDL_Window *init_sdl3_window(int w, int h, const char *title);
void update_window_size(int w, int h);
void SwapWindowSynced();