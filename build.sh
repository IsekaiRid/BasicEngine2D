#!/usr/bin/env bash
set -e

BUILD_DIR="build"
OUTPUT="$BUILD_DIR/main.exe"

# Mode: jalanin "./build.sh debug" buat debug, atau "./build.sh" / "./build.sh release" buat release
BUILD_MODE="${1:-release}"

INCLUDES=(
    "-Ithird_party"
    "-Ithird_party/GLAD/include"
    "-Ithird_party/SDL/include"
    "-Ithird_party/GLM"
    "-Ithird_party/GLM/glm"
    "-Ithird_party/GLAD/src"
    "-Isrc"
)

GLAD_C="third_party/GLAD/src/glad.c"
MAIN_CPP="src/main.cpp"

LIB_DIRS=(
    "-Lthird_party/SDL/lib/x64"
)

LIBS=(
    "-lSDL3"
    "-lSDL3_image"
    "-lopengl32"
    "-lgdi32"
    "-luser32"
    "-lshell32"
    "-lwinmm"
    "-lole32"
    "-luuid"
    "-lsetupapi"
    "-limm32"
    "-lversion"
)

WARNINGS=(
    "-Wdeprecated"
    "-Wno-deprecated-declarations"
    "-Wformat-security"
    "-Wno-format-security"
)

LINKER_FLAGS=(
    "-Wl,/SUBSYSTEM:CONSOLE"
)

# === PILIH MODE ===
if [ "$BUILD_MODE" = "debug" ]; then
    OPT_FLAGS=("-g" "-O0")
    echo "🔧 Building DEBUG (no optimization, dengan simbol debug)..."
elif [ "$BUILD_MODE" = "release" ]; then
    OPT_FLAGS=("-O2" "-DNDEBUG")
    echo "🚀 Building RELEASE (optimized, buat ukur performa/CPU asli)..."
else
    echo "❌ Mode tidak dikenal: '$BUILD_MODE' (pakai 'debug' atau 'release')"
    exit 1
fi

# === BUILD ===
mkdir -p "$BUILD_DIR"

clang++ \
    "${OPT_FLAGS[@]}" \
    "${INCLUDES[@]}" \
    "$MAIN_CPP" \
    "$GLAD_C" \
    -o "$OUTPUT" \
    "${LIB_DIRS[@]}" \
    "${LIBS[@]}" \
    "${WARNINGS[@]}" \
    "${LINKER_FLAGS[@]}"

if [ $? -eq 0 ]; then
    echo "✅ Build success ($BUILD_MODE): $OUTPUT"
    if [ -d "assets" ]; then
        mkdir -p "$BUILD_DIR/assets"
        cp -r assets/. "$BUILD_DIR/assets/"
        echo "📁 Assets disalin ke $BUILD_DIR/assets"
    else
        echo "⚠️  Folder 'assets' tidak ditemukan di root project, dilewati"
    fi
else
    echo "❌ Build failed!"
    exit 1
fi