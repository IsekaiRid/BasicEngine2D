#pragma once

#include <Basic2D/Engine.h>

struct RenderWindows
{
    glm::vec2 ScreenSize;
};

static RenderWindows renderWindows;
static SDL_Window *sdlWindow = nullptr;
static SDL_GLContext glContext = nullptr;

SDL_Window *init_sdl3_window(int w, int h, const char *title);
void update_window_size(int w, int h);