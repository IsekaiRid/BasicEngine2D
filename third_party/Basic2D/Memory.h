#pragma once
#include <Basic2D/Engine.h>
#include <cstdlib>
#include <cstring>
#include <cstdint>

struct Memory
{
    size_t capacity; 
    size_t used;     
    uint8_t *poin;   
};

Memory Create_Alloc_Memory(size_t totalSize)
{
    Memory memory = {};

    memory.poin = (uint8_t *)malloc(totalSize);
    LOG_ASSERT(memory.poin, "Failed to allocate memory.");

    if (memory.poin)
        memset(memory.poin, 0, totalSize);

    memory.capacity = totalSize;
    memory.used = 0;

    return memory;
}

void *Get_Alloc_Memory(Memory *memory, size_t totalSize)
{
    LOG_ASSERT(memory, "Memory is null.");

    void *result = nullptr;
    size_t rawSize = totalSize + 1;
    size_t alignedSize = (rawSize + 7) & ~7;

    if (memory->used + alignedSize <= memory->capacity)
    {
        result = memory->poin + memory->used;
        memory->used += alignedSize;
    }
    else
    {
        LOG_ASSERT(false, "Failed Get Alloc Memory: Memory Capacity Exceeded!");
    }

    return result;
}

void Reset_Alloc_Memory(Memory *memory)
{
    if (memory)
    {
        memory->used = 0;
    }
}

void Free_Alloc_Memory(Memory *memory)
{
    if (memory && memory->poin)
    {
        free(memory->poin);
        memory->poin = nullptr;
        memory->capacity = 0;
        memory->used = 0;
    }
}