#pragma once

#define RGB(r, g, b) (r) / 255.0f, (g) / 255.0f, (b) / 255.0f
#define RGBA(r, g, b, a) (r) / 255.0f, (g) / 255.0f, (b) / 255.0f, (a) / 255.0f

#define KB(x) ((size_t)(x) * 1024ULL)
#define MB(x) ((size_t)(x) * 1024ULL * 1024ULL)
#define GB(x) ((size_t)(x) * 1024ULL * 1024ULL * 1024ULL)
#define TB(x) ((size_t)(x) * 1024ULL * 1024ULL * 1024ULL * 1024ULL)