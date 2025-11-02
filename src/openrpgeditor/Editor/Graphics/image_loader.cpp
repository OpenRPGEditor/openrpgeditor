#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include <stdbool.h>
#include <stdio.h>

#if _WIN32
#define STBI_WINDOWS_UTF8
#endif

#include "Editor/Log.hpp"

#include <SDL3_image/SDL_image.h>
#ifdef __cplusplus
extern "C" {
#endif
bool LoadTextureFromFile(const char* filename, SDL_Texture** texture_ptr, int* width, int* height, SDL_Renderer* renderer) {
  const auto surface = IMG_Load(filename);
  if (!surface) {
    APP_ERROR("Failed to load image '{}': reason: {}\n", filename, SDL_GetError());
    return false;
  }
  const auto convertedSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_ABGR8888);
  SDL_DestroySurface(surface);
  if (!convertedSurface) {
    APP_ERROR("Failed to convert surface to ABGR8888 for image '{}': reason: {}\n", filename, SDL_GetError());
    return false;
  }
  const auto texture = SDL_CreateTextureFromSurface(renderer, convertedSurface);
  SDL_DestroySurface(convertedSurface);
  if (!texture) {
    APP_ERROR("Failed to create texture for image '{}': reason: {}\n", filename, SDL_GetError());
    return false;
  }
  float _width = 0, _height = 0;
  SDL_GetTextureSize(texture, &_width, &_height);
  *width = _width;
  *height = _height;
  *texture_ptr = texture;
  return true;
}

bool LoadTextureFromMemory(const void* inData, int len, SDL_Texture** texture_ptr, int* width, int* height, SDL_Renderer* renderer) {
  // int channels = 0;
  // unsigned char* data = stbi_load_from_memory(inData, len, width, height, &channels, 0);
  //
  // if (data == NULL) {
  //   fprintf(stderr, "Failed to load image: reason: %s\n", stbi_failure_reason());
  //   return false;
  // }
  //
  // return LoadTexturePriv(texture_ptr, width, height, renderer, channels, data);
  return false;
}

#ifdef __cplusplus
}
#endif