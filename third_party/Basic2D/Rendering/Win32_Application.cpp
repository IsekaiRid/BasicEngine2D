
#include <Basic2D/Rendering/Win32_Application.h>

SDL_Window *init_sdl3_window(int w, int h, const char *title)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        LOG_ERROR("SDL_Init failed: %s", SDL_GetError());
        return nullptr;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    sdlWindow = SDL_CreateWindow(
        title,
        w, h,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!sdlWindow)
    {
        LOG_ERROR("SDL_CreateWindow failed: %s", SDL_GetError());
        return nullptr;
    }

    glContext = SDL_GL_CreateContext(sdlWindow);
    if (!glContext)
    {
        LOG_ERROR("SDL_GL_CreateContext failed: %s", SDL_GetError());
        return nullptr;
    }

    SDL_GL_SetSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        return nullptr;
    }

    renderWindows.ScreenSize.x = static_cast<float>(w);
    renderWindows.ScreenSize.y = static_cast<float>(h);

    return sdlWindow;
}

void update_window_size(int w, int h)
{
    glViewport(0, 0, w, h);
    renderWindows.ScreenSize.x = static_cast<float>(w);
    renderWindows.ScreenSize.y = static_cast<float>(h);
}