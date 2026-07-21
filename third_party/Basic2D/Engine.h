#pragma once

// External dependencies
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Core utility headers
#include <Basic2D/Core/Config.h>
#include <Basic2D/Core/Logger.h>
#include <Basic2D/Core/Memory.h>
#include <Basic2D/Core/File.h>
#include <Basic2D/Core/Time.h>
#include <Basic2D/Core/Collision.h>
#include <Basic2D/Core/Camera2D.h>
#include <Basic2D/Core/Assets.h>
#include <Basic2D/Core/Sprite.h>

// Rendering layer
#include <Basic2D/Rendering/StructRender.h>
#include <Basic2D/Rendering/Win_SDL/Win32_SDL.h>
#include <Basic2D/Rendering/Win_SDL/LoadTextureSDL/LoadSDLTexture.h>
#include <Basic2D/Rendering/Win_SDL/Input/SDL_Input.h>
#include <Basic2D/Rendering/Win_SDL/Font/SDL_Font.h>
#include <Basic2D/Rendering/OpenGL/Render.h>

// Tooling helpers
#include <Basic2D/Tools/DirectionUtils.h>
#include <Basic2D/Tools/RulerTool.h>
