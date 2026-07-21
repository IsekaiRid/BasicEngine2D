#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Basic2D/Rendering/Win_SDL/Win32_SDL.h>

struct Camera2D
{
    glm::vec2 pos = {0.0f, 0.0f};
    glm::vec2 zoom = {1.0f, 1.0f};
    bool active = true;
    glm::vec2 targetResolution = {0.0f, 0.0f};
};

static Camera2D gameCam = {{0.0f, 0.0f}, {1.0f, 1.0f}, true, {0.0f, 0.0f}};
static Camera2D uiCam = {{0.0f, 0.0f}, {1.0f, 1.0f}, true, {0.0f, 0.0f}};

inline glm::mat4 GetScreenOrtho()
{
    return glm::ortho(
        0.0f, renderWindows.ScreenSize.x,
        renderWindows.ScreenSize.y, 0.0f,
        -1.0f, 1.0f);
}

inline glm::mat4 GetCameraViewProjection(const Camera2D &camera, unsigned int shaderProgram = 0)
{
    if (!camera.active)
    {
        glm::mat4 ortho = GetScreenOrtho();
        if (shaderProgram != 0)
        {
            int loc = glGetUniformLocation(shaderProgram, "ViewProjection");
            if (loc != -1)
            {
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(ortho));
            }
        }
        return ortho;
    }
    glm::vec2 baseSize = (camera.targetResolution.x > 0.0f && camera.targetResolution.y > 0.0f)
                             ? camera.targetResolution
                             : renderWindows.ScreenSize;
    glm::mat4 projection = glm::ortho(0.0f, baseSize.x, baseSize.y, 0.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-camera.pos, 0.0f));
    glm::mat4 zoom = glm::scale(glm::mat4(1.0f), glm::vec3(camera.zoom, 1.0f));
    glm::mat4 vp = projection * view * zoom;

    if (shaderProgram != 0)
    {
        int loc = glGetUniformLocation(shaderProgram, "ViewProjection");
        if (loc != -1)
        {
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(vp));
        }
    }

    return vp;
}