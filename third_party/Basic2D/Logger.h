#pragma once
#include <utility>
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK() __builtin_trap()
#endif

enum TypeColor
{
    WHITE = 37,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
};

template <typename... Args>
void Log(const char *prefix, const char *fmt, TypeColor color, Args &&...args)
{
    char buffer[4096];

    std::snprintf(
        buffer,
        sizeof(buffer),
        fmt,
        std::forward<Args>(args)...);

    std::printf(
        "\033[1;%dm[%s] %s\033[0m\n",
        color,
        prefix,
        buffer);
}

#define LOG_TRACE(msg, ...) Log("TRACE", msg, GREEN, ##__VA_ARGS__)
#define LOG_WARN(msg, ...) Log("WARN", msg, YELLOW, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) Log("ERROR", msg, RED, ##__VA_ARGS__)

#define LOG_ASSERT(condition, msg, ...)         \
    if (!(condition))                           \
    {                                           \
        Log("ASSERT", msg, RED, ##__VA_ARGS__); \
        DEBUG_BREAK();                          \
        std::abort();                           \
    }