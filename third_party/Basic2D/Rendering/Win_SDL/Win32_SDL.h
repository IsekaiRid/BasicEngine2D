#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <Basic2D/Core/Logger.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <Basic2D/Tools/InfManamentPerformant.h>
#endif

struct RenderWindows
{
    glm::vec2 ScreenSize;
};

extern RenderWindows renderWindows;
extern SDL_Window *sdlWindow;
extern SDL_GLContext glContext;

extern bool g_UseDwmFlush;

SDL_Window *init_sdl3_window(int w, int h, const char *title);
void update_window_size(int w, int h);
void SwapWindowSynced();