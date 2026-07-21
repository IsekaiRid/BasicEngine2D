#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <Basic2D/Core/Logger.h>
#include <Basic2D/Rendering/Win_SDL/Win32_SDL.h>
#include <string>
#include <unordered_map>

struct TextTexture
{
    GLuint textureId = 0;
    int width = 0;
    int height = 0;
};

struct TextQuadRenderer
{
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint programId = 0;
    int screenSizeLoc = -1;
    int posLoc = -1;
    int sizeLoc = -1;
};

static TextQuadRenderer textRenderer;
static std::unordered_map<std::string, TextTexture> g_TextCache;

static const char *TextVertSrc = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
out vec2 vUV;
uniform vec2 ScreenSize;
uniform vec2 QuadPos;
uniform vec2 QuadSize;
void main()
{
    vec2 pixelPos = QuadPos + aPos * QuadSize;
    vec2 ndc = (pixelPos / ScreenSize) * 2.0 - 1.0;
    ndc.y = -ndc.y;
    gl_Position = vec4(ndc, 0.0, 1.0);
    vUV = aUV;
}
)";

static const char *TextFragSrc = R"(
#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D TextTex;
void main()
{
    FragColor = texture(TextTex, vUV);
}
)";

inline GLuint CompileTextShaderProgram()
{
    int success;
    char infoLog[512];

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &TextVertSrc, nullptr);
    glCompileShader(vert);
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vert, sizeof(infoLog), nullptr, infoLog);
        LOG_ERROR("Text Vertex Shader Failed:\n%s", infoLog);
    }

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &TextFragSrc, nullptr);
    glCompileShader(frag);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(frag, sizeof(infoLog), nullptr, infoLog);
        LOG_ERROR("Text Fragment Shader Failed:\n%s", infoLog);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        LOG_ERROR("Text Shader Program Link Failed:\n%s", infoLog);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

inline void Text_Init()
{
    textRenderer.programId = CompileTextShaderProgram();
    textRenderer.screenSizeLoc = glGetUniformLocation(textRenderer.programId, "ScreenSize");
    textRenderer.posLoc = glGetUniformLocation(textRenderer.programId, "QuadPos");
    textRenderer.sizeLoc = glGetUniformLocation(textRenderer.programId, "QuadSize");

    float quadVerts[] = {
        // pos      // uv
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
    };

    glGenVertexArrays(1, &textRenderer.VAO);
    glGenBuffers(1, &textRenderer.VBO);
    glBindVertexArray(textRenderer.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, textRenderer.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

inline TextTexture &GetOrCreateTextTexture(const std::string &text, SDL_Color color)
{
    auto found = g_TextCache.find(text);
    if (found != g_TextCache.end())
        return found->second;

    const int charW = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
    const int charH = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
    int w = (int)text.size() * charW;
    int h = charH;
    if (w <= 0)
        w = charW;

    SDL_Surface *surface = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA32);
    SDL_Renderer *swRenderer = SDL_CreateSoftwareRenderer(surface);

    SDL_SetRenderDrawColor(swRenderer, 0, 0, 0, 0);
    SDL_RenderClear(swRenderer);

    SDL_SetRenderDrawColor(swRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderDebugText(swRenderer, 0.0f, 0.0f, text.c_str());
    SDL_RenderPresent(swRenderer);

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_DestroyRenderer(swRenderer);
    SDL_DestroySurface(surface);

    TextTexture tt;
    tt.textureId = texId;
    tt.width = w;
    tt.height = h;

    g_TextCache[text] = tt;
    return g_TextCache[text];
}

inline void DrawText(const std::string &text, glm::vec2 position, glm::vec2 scale = {1.0f, 1.0f},
                     SDL_Color color = {255, 255, 255, 255})
{
    TextTexture &tex = GetOrCreateTextTexture(text, color);

    glUseProgram(textRenderer.programId);
    glUniform2f(textRenderer.screenSizeLoc, renderWindows.ScreenSize.x, renderWindows.ScreenSize.y);
    glUniform2f(textRenderer.posLoc, position.x, position.y);
    glUniform2f(textRenderer.sizeLoc, tex.width * scale.x, tex.height * scale.y);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.textureId);

    glBindVertexArray(textRenderer.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

inline void Text_Clean()
{
    for (auto &pair : g_TextCache)
        glDeleteTextures(1, &pair.second.textureId);
    g_TextCache.clear();

    glDeleteBuffers(1, &textRenderer.VBO);
    glDeleteVertexArrays(1, &textRenderer.VAO);
    glDeleteProgram(textRenderer.programId);
}