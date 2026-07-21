#include <Basic2D/Engine.h>
#include "games.cpp"

const unsigned int w = 800;
const unsigned int h = 600;

int main()
{
    Memory m = Create_Alloc_Memory(MB(1));

    SDL_Window *window = init_sdl3_window(w, h, "Games");

    if (!window)
        return -1;

    Collision_Shader_Init("assets/shader/collision_debug.vert", "assets/shader/collision_debug.frag");

    Create_Shader_Init(&m, "assets/shader/default.vert", "assets/shader/default.frag");

    Text_Init();

    uiCam.targetResolution = {0.0f, 0.0f};
    uiCam.active = false;

    // Collision_Debug_Enable();
    // Ruler_Enable();
    rendy_on_game();

    bool running = true;
    SDL_Event event;

    while (running)
    {
        UpdateTime();

        while (SDL_PollEvent(&event))
        {
            ProcessInputEvent(event);

            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;

            case SDL_EVENT_WINDOW_RESIZED:
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                update_window_size(event.window.data1, event.window.data2);
                break;

            default:
                break;
            }
        }

        UpdateInput();

        glClearColor(RGBA(30, 30, 30, 255));
        glClear(GL_COLOR_BUFFER_BIT);

        update_ui();
        update_game();
        render_shader();
        Ruler_RenderVisual();

        SwapWindowSynced();
        PostUpdateInput();
    }

    render_clean();
    Text_Clean();
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}