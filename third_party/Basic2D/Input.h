#pragma once

#include <Basic2D/Engine.h>
#include <SDL3/SDL.h>

enum class ActionCode
{
    A = SDL_SCANCODE_A, B = SDL_SCANCODE_B, C = SDL_SCANCODE_C, D = SDL_SCANCODE_D,
    E = SDL_SCANCODE_E, F = SDL_SCANCODE_F, G = SDL_SCANCODE_G, H = SDL_SCANCODE_H,
    I = SDL_SCANCODE_I, J = SDL_SCANCODE_J, K = SDL_SCANCODE_K, L = SDL_SCANCODE_L,
    M = SDL_SCANCODE_M, N = SDL_SCANCODE_N, O = SDL_SCANCODE_O, P = SDL_SCANCODE_P,
    Q = SDL_SCANCODE_Q, R = SDL_SCANCODE_R, S = SDL_SCANCODE_S, T = SDL_SCANCODE_T,
    U = SDL_SCANCODE_U, V = SDL_SCANCODE_V, W = SDL_SCANCODE_W, X = SDL_SCANCODE_X,
    Y = SDL_SCANCODE_Y, Z = SDL_SCANCODE_Z,

    Num0 = SDL_SCANCODE_0, Num1 = SDL_SCANCODE_1, Num2 = SDL_SCANCODE_2, Num3 = SDL_SCANCODE_3,
    Num4 = SDL_SCANCODE_4, Num5 = SDL_SCANCODE_5, Num6 = SDL_SCANCODE_6, Num7 = SDL_SCANCODE_7,
    Num8 = SDL_SCANCODE_8, Num9 = SDL_SCANCODE_9,

    Up = SDL_SCANCODE_UP, Down = SDL_SCANCODE_DOWN,
    Left = SDL_SCANCODE_LEFT, Right = SDL_SCANCODE_RIGHT,

    LeftShift = SDL_SCANCODE_LSHIFT, RightShift = SDL_SCANCODE_RSHIFT,
    LeftCtrl = SDL_SCANCODE_LCTRL, RightCtrl = SDL_SCANCODE_RCTRL,
    LeftAlt = SDL_SCANCODE_LALT, RightAlt = SDL_SCANCODE_RALT,

    Space = SDL_SCANCODE_SPACE,
    Enter = SDL_SCANCODE_RETURN,
    Tab = SDL_SCANCODE_TAB,
    Backspace = SDL_SCANCODE_BACKSPACE,
    Escape = SDL_SCANCODE_ESCAPE,

    F1 = SDL_SCANCODE_F1, F2 = SDL_SCANCODE_F2, F3 = SDL_SCANCODE_F3, F4 = SDL_SCANCODE_F4,
    F5 = SDL_SCANCODE_F5, F6 = SDL_SCANCODE_F6, F7 = SDL_SCANCODE_F7, F8 = SDL_SCANCODE_F8,
    F9 = SDL_SCANCODE_F9, F10 = SDL_SCANCODE_F10, F11 = SDL_SCANCODE_F11, F12 = SDL_SCANCODE_F12,

    MouseLeft = 1000,
    MouseRight = 1001,
    MouseMiddle = 1002
};

struct InputData
{
    glm::vec2 mousePos{};
    glm::vec2 scrollDelta{};
    bool currentKeyStates[SDL_SCANCODE_COUNT] = {};
    bool lastKeyStates[SDL_SCANCODE_COUNT] = {};
    bool currentMouseStates[8] = {};
    bool lastMouseStates[8] = {};
};

static InputData input;

static bool IsMouseCode(ActionCode code)
{
    switch (code)
    {
    case ActionCode::MouseLeft:
    case ActionCode::MouseRight:
    case ActionCode::MouseMiddle:
        return true;
    default:
        return false;
    }
}

static int GetMouseIndex(ActionCode code)
{
    switch (code)
    {
    case ActionCode::MouseLeft:   return SDL_BUTTON_LEFT;   // 1
    case ActionCode::MouseRight:  return SDL_BUTTON_RIGHT;  // 3
    case ActionCode::MouseMiddle: return SDL_BUTTON_MIDDLE; // 2
    default: return 0;
    }
}

static void InitInput()
{
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++)
    {
        input.currentKeyStates[i] = false;
        input.lastKeyStates[i] = false;
    }
    for (int i = 0; i < 8; i++)
    {
        input.currentMouseStates[i] = false;
        input.lastMouseStates[i] = false;
    }
    input.mousePos = glm::vec2(0.0f);
    input.scrollDelta = glm::vec2(0.0f);
}

static void ProcessInputEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_EVENT_KEY_DOWN:
        if (event.key.scancode < SDL_SCANCODE_COUNT && event.key.scancode >= 0)
        {
            input.currentKeyStates[event.key.scancode] = true;
        }
        break;

    case SDL_EVENT_KEY_UP:
        if (event.key.scancode < SDL_SCANCODE_COUNT && event.key.scancode >= 0)
        {
            input.currentKeyStates[event.key.scancode] = false;
        }
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event.button.button < 8 && event.button.button >= 0)
        {
            input.currentMouseStates[event.button.button] = true;
        }
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event.button.button < 8 && event.button.button >= 0)
        {
            input.currentMouseStates[event.button.button] = false;
        }
        break;

    case SDL_EVENT_MOUSE_WHEEL:
        input.scrollDelta.x += event.wheel.x;
        input.scrollDelta.y += event.wheel.y;
        break;
    }
}

static void UpdateInput()
{
    float mx, my;
    SDL_GetMouseState(&mx, &my);
    input.mousePos = glm::vec2(mx, my);
}

static void PostUpdateInput()
{
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++)
        input.lastKeyStates[i] = input.currentKeyStates[i];
    for (int i = 0; i < 8; i++)
        input.lastMouseStates[i] = input.currentMouseStates[i];

    input.scrollDelta = glm::vec2(0.0f);
}

static bool Held(ActionCode code)
{
    if (IsMouseCode(code))
    {
        int idx = GetMouseIndex(code);
        return idx > 0 && idx < 8 && input.currentMouseStates[idx];
    }

    int sc = static_cast<int>(code);
    return sc >= 0 && sc < SDL_SCANCODE_COUNT && input.currentKeyStates[sc];
}

static bool Pressed(ActionCode code)
{
    if (IsMouseCode(code))
    {
        int idx = GetMouseIndex(code);
        if (idx <= 0 || idx >= 8) return false;
        return input.currentMouseStates[idx] && !input.lastMouseStates[idx];
    }
    int sc = static_cast<int>(code);
    if (sc < 0 || sc >= SDL_SCANCODE_COUNT) return false;
    return input.currentKeyStates[sc] && !input.lastKeyStates[sc];
}

static bool Released(ActionCode code)
{
    if (IsMouseCode(code))
    {
        int idx = GetMouseIndex(code);
        if (idx <= 0 || idx >= 8) return false;
        return !input.currentMouseStates[idx] && input.lastMouseStates[idx];
    }
    int sc = static_cast<int>(code);
    if (sc < 0 || sc >= SDL_SCANCODE_COUNT) return false;
    return !input.currentKeyStates[sc] && input.lastKeyStates[sc];
}

static glm::vec2 MousePos()
{
    return input.mousePos;
}

static glm::vec2 ScrollDelta()
{
    return input.scrollDelta;
}