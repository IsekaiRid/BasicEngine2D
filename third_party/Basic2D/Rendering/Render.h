#pragma once

#include <Basic2D/Engine.h>

inline void Create_Shader_Init(Memory *tempMemory, const char *FilePathVertex, const char *FilePathFragman)
{
    LOG_ASSERT(tempMemory, "Memory allocator is null.");

    int success;
    char infoLog[512];

    size_t sizeVertex = FileSize(FilePathVertex);
    char *vertexSource = (char *)Get_Alloc_Memory(tempMemory, sizeVertex);

    FileRead(FilePathVertex, vertexSource, sizeVertex);
    vertexSource[sizeVertex] = '\0';

    shaderData.program.vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderData.program.vertexShaderId, 1, (const char **)&vertexSource, nullptr);
    glCompileShader(shaderData.program.vertexShaderId);

    glGetShaderiv(shaderData.program.vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderData.program.vertexShaderId, sizeof(infoLog), nullptr, infoLog);
        LOG_ERROR("Vertex Shader Compilation Failed:\n%s", infoLog);
    }

    size_t sizeFragment = FileSize(FilePathFragman);
    char *fragmentSource = (char *)Get_Alloc_Memory(tempMemory, sizeFragment);

    FileRead(FilePathFragman, fragmentSource, sizeFragment);
    fragmentSource[sizeFragment] = '\0';

    shaderData.program.fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderData.program.fragmentShaderId, 1, (const char **)&fragmentSource, nullptr);
    glCompileShader(shaderData.program.fragmentShaderId);

    glGetShaderiv(shaderData.program.fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderData.program.fragmentShaderId, sizeof(infoLog), nullptr, infoLog);
        LOG_ERROR("Fragment Shader Compilation Failed:\n%s", infoLog);
    }

    shaderData.program.programId = glCreateProgram();
    glAttachShader(shaderData.program.programId, shaderData.program.vertexShaderId);
    glAttachShader(shaderData.program.programId, shaderData.program.fragmentShaderId);
    glLinkProgram(shaderData.program.programId);

    glGetProgramiv(shaderData.program.programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderData.program.programId, sizeof(infoLog), nullptr, infoLog);
        LOG_ERROR("Shader Program Linking Failed:\n%s", infoLog);
    }

    glDeleteShader(shaderData.program.vertexShaderId);
    glDeleteShader(shaderData.program.fragmentShaderId);

    glGenVertexArrays(1, &shaderData.sprite.VAO);
    shaderData.sprite.screenSizeLoc = glGetUniformLocation(shaderData.program.programId, "ScreenSize");

    glGenBuffers(1, &shaderData.sprite.spriteSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderData.sprite.spriteSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SpriteData) * 100, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shaderData.sprite.spriteSSBO);

    glGenBuffers(1, &shaderData.sprite.uiSpriteSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderData.sprite.uiSpriteSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SpriteData) * 100, nullptr, GL_DYNAMIC_DRAW);

    bool atlasOk = BuildTextureArray(AtlasFilePaths, ATLAS_COUNT,
                                     &shaderData.atlas.textureArrayId,
                                     shaderData.atlas.sizes);

    if (!atlasOk)
    {
        LOG_ERROR("Create_Shader_Init: Failed to build texture atlas array");
        glDeleteBuffers(1, &shaderData.sprite.spriteSSBO);
        glDeleteBuffers(1, &shaderData.sprite.uiSpriteSSBO);
        glDeleteVertexArrays(1, &shaderData.sprite.VAO);
        glDeleteProgram(shaderData.program.programId);
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

inline void render_shader()
{
    glUseProgram(shaderData.program.programId);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, shaderData.atlas.textureArrayId);

    glUniform2f(shaderData.sprite.screenSizeLoc, renderWindows.ScreenSize.x, renderWindows.ScreenSize.y);
    glBindVertexArray(shaderData.sprite.VAO);

    GetCameraViewProjection(gameCam, shaderData.program.programId);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderData.sprite.spriteSSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                    sizeof(SpriteData) * spriteRender.amount,
                    spriteRender.spriteData);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shaderData.sprite.spriteSSBO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, spriteRender.amount);

    GetCameraViewProjection(uiCam, shaderData.program.programId);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderData.sprite.uiSpriteSSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                    sizeof(SpriteData) * uiSpriteRender.amount,
                    uiSpriteRender.spriteData);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, shaderData.sprite.uiSpriteSSBO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, uiSpriteRender.amount);

    render_Collision_update();

    glBindVertexArray(0);

    ClearWorldColliders();
    ResetSpriteRender();
}

inline void render_clean()
{
    glDeleteBuffers(1, &shaderData.sprite.spriteSSBO);
    glDeleteBuffers(1, &shaderData.sprite.uiSpriteSSBO);
    glDeleteTextures(1, &shaderData.atlas.textureArrayId);
    glDeleteProgram(shaderData.program.programId);

    glDeleteVertexArrays(1, &shaderData.sprite.VAO);
}