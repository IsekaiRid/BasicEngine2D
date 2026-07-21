#include <Basic2D/Rendering/Win32_Application.h>


#if defined(_WIN32)

static ULARGE_INTEGER g_LastCPU = {};
static ULARGE_INTEGER g_LastSysCPU = {};
static ULARGE_INTEGER g_LastUserCPU = {};
static int g_NumProcessors = 1;
static HANDLE g_ProcessHandle = nullptr;
static bool g_CpuInited = false;

static void InitCPUUsageTracking()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    g_NumProcessors = sysInfo.dwNumberOfProcessors;

    g_ProcessHandle = GetCurrentProcess();

    FILETIME ftime, fsys, fuser;
    GetSystemTimeAsFileTime(&ftime);
    memcpy(&g_LastCPU, &ftime, sizeof(FILETIME));

    GetProcessTimes(g_ProcessHandle, &ftime, &ftime, &fsys, &fuser);
    memcpy(&g_LastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&g_LastUserCPU, &fuser, sizeof(FILETIME));

    g_CpuInited = true;
}

float GetProcessCPUUsagePercent()
{
    if (!g_CpuInited)
        InitCPUUsageTracking();

    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(g_ProcessHandle, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));

    ULONGLONG sysDelta = (sys.QuadPart - g_LastSysCPU.QuadPart) +
                         (user.QuadPart - g_LastUserCPU.QuadPart);
    ULONGLONG timeDelta = now.QuadPart - g_LastCPU.QuadPart;

    float percent = 0.0f;
    if (timeDelta > 0)
    {
        percent = (float)((double)sysDelta / (double)timeDelta / g_NumProcessors * 100.0);
    }

    g_LastCPU = now;
    g_LastSysCPU = sys;
    g_LastUserCPU = user;

    return percent;
}

#endif

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

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        return nullptr;
    }

#if defined(_WIN32)
    BOOL compositionEnabled = FALSE;
    if (SUCCEEDED(DwmIsCompositionEnabled(&compositionEnabled)) && compositionEnabled)
    {
        SDL_GL_SetSwapInterval(0);
        g_UseDwmFlush = true;
    }
    else
    {
        SDL_GL_SetSwapInterval(1);
        g_UseDwmFlush = false;
    }
#else
    SDL_GL_SetSwapInterval(1);
    g_UseDwmFlush = false;
#endif

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

void SwapWindowSynced()
{
#if defined(_WIN32)
    if (g_UseDwmFlush)
    {
        DwmFlush();
    }
#endif
    SDL_GL_SwapWindow(sdlWindow);
}