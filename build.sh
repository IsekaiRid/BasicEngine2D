BUILD_DIR="build"
OUTPUT="$BUILD_DIR/main.exe"

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

# === BUILD ===
echo "Building..."
mkdir -p "$BUILD_DIR"

clang++ -g \
    "${INCLUDES[@]}" \
    "$MAIN_CPP" \
    "$GLAD_C" \
    -o "$OUTPUT" \
    "${LIB_DIRS[@]}" \
    "${LIBS[@]}" \
    "${WARNINGS[@]}" \
    "${LINKER_FLAGS[@]}"

if [ $? -eq 0 ]; then
    echo "✅ Build success: $OUTPUT"
else
    echo "❌ Build failed!"
fi