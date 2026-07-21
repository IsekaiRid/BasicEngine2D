#pragma once

#include <Basic2D/Core/Logger.h>
#include <Basic2D/Core/Memory.h>
#include <stdio.h>

size_t FileSize(const char *filePath)
{
    LOG_ASSERT(filePath, "File path is null.");

    FILE *file = fopen(filePath, "rb");
    if (!file)
    {
        LOG_ERROR("Failed to open file: %s", filePath);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fclose(file);

    return size;
};

char *FileRead(const char *filePath, char *buffer, size_t size)
{
    LOG_ASSERT(filePath, "File path is null.");
    LOG_ASSERT(buffer, "Buffer is null.");

    FILE *file = fopen(filePath, "rb");
    if (!file)
    {
        LOG_ERROR("Failed to open file: %s", filePath);
        return nullptr;
    }

    size_t bytesRead = fread(buffer, 1, size, file);
    fclose(file);

    buffer[bytesRead] = '\0';

    return buffer;
};

char *FileReadAllocMemory(const char *filePath, size_t size, Memory *memory)
{
    LOG_ASSERT(filePath, "File path is null.");
    LOG_ASSERT(memory, "Memory is null.");

    char *buffer = (char *)Get_Alloc_Memory(memory, size + 1);
    if (!buffer)
    {
        LOG_ERROR("Failed to allocate memory for file: %s", filePath);
        return nullptr;
    }

    FileRead(filePath, buffer, size);

    return buffer;
};
