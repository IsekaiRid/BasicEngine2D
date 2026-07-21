#pragma once

#include <glm/glm.hpp>
#include <Basic2D/Rendering/Win_SDL/Input/SDL_Input.h>

static glm::vec2 GetVector(ActionCode negativeX, ActionCode positiveX, ActionCode negativeY, ActionCode positiveY)
{
    glm::vec2 result = glm::vec2(0.0f, 0.0f);

    if (Held(negativeX))
        result.x -= 1.0f;
    if (Held(positiveX))
        result.x += 1.0f;

    if (Held(negativeY))
        result.y += 1.0f;
    if (Held(positiveY))
        result.y -= 1.0f;

    if (glm::length(result) > 0.0f)
    {
        result = glm::normalize(result);
    }

    return result;
}
