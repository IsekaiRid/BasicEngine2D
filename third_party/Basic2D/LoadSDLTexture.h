#pragma once

#include <Basic2D/Engine.h>

SDL_Surface *LoadImageSurface(const char *filePath)
{
    SDL_Surface *rawSurface = IMG_Load(filePath);
    if (!rawSurface)
    {
        LOG_ERROR("LoadImageSurface: Failed to load '%s' — %s", filePath, SDL_GetError());
        return nullptr;
    }

    SDL_Surface *rgbaSurface = SDL_ConvertSurface(rawSurface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(rawSurface);

    if (!rgbaSurface)
    {
        LOG_ERROR("LoadImageSurface: Failed to convert '%s' to RGBA — %s", filePath, SDL_GetError());
        return nullptr;
    }

    return rgbaSurface;
}
inline bool BuildTextureArray(const char *const *paths, int count,
                              GLuint *outTexId, glm::vec2 *outSizes)
{
    SDL_Surface **surfaces = (SDL_Surface **)calloc(count, sizeof(SDL_Surface *));
    if (!surfaces)
    {
        LOG_ERROR("BuildTextureArray: Out of memory");
        return false;
    }

    int maxW = 1, maxH = 1;
    bool ok = true;

    for (int i = 0; i < count; i++)
    {
        surfaces[i] = LoadImageSurface(paths[i]);
        if (!surfaces[i])
        {
            ok = false;
            break;
        }

        if (surfaces[i]->w > maxW)
            maxW = surfaces[i]->w;
        if (surfaces[i]->h > maxH)
            maxH = surfaces[i]->h;

        outSizes[i] = {(float)surfaces[i]->w, (float)surfaces[i]->h};
    }

    if (!ok)
    {
        for (int j = 0; j < count; j++)
            if (surfaces[j])
                SDL_DestroySurface(surfaces[j]);
        free(surfaces);
        return false;
    }

    glGenTextures(1, outTexId);
    glBindTexture(GL_TEXTURE_2D_ARRAY, *outTexId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, maxW, maxH, count,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    for (int i = 0; i < count; i++)
    {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                        0, 0, i,
                        surfaces[i]->w, surfaces[i]->h, 1,
                        GL_RGBA, GL_UNSIGNED_BYTE,
                        surfaces[i]->pixels);
    }

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    for (int i = 0; i < count; i++)
        SDL_DestroySurface(surfaces[i]);
    free(surfaces);

    return true;
}