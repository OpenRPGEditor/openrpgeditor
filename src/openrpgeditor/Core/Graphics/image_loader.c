#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include <stdbool.h>
#include <stdio.h>

#if _WIN32
#define STBI_WINDOWS_UTF8
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static bool LoadTexturePriv(SDL_Texture** texture_ptr, const int* width, const int* height, SDL_Renderer* renderer, const int channels, void* data) {
  SDL_Surface* surface = SDL_CreateSurfaceFrom(*width, *height, SDL_PIXELFORMAT_ABGR8888, data, channels * *width);
  if (surface == NULL) {
    fprintf(stderr, "Failed to create SDL surface: %s\n", SDL_GetError());
    return false;
  }

  *texture_ptr = SDL_CreateTextureFromSurface(renderer, surface);

  if ((*texture_ptr) == NULL) {
    fprintf(stderr, "Failed to create SDL texture: %s\n", SDL_GetError());
    SDL_DestroySurface(surface);
    stbi_image_free(data);
    return false;
  }

  SDL_SetTextureBlendMode(*texture_ptr, SDL_BLENDMODE_BLEND);
  SDL_SetTextureScaleMode(*texture_ptr, SDL_SCALEMODE_NEAREST);

  SDL_DestroySurface(surface);
  stbi_image_free(data);
  return true;
}

bool LoadTextureFromFile(const char* filename, SDL_Texture** texture_ptr, int* width, int* height, SDL_Renderer* renderer) {
  int channels = 0;
  unsigned char* data = stbi_load(filename, width, height, &channels, 0);

  if (data == NULL) {
    fprintf(stderr, "Failed to load image: %s, reason: %s\n", filename, stbi_failure_reason());
    return false;
  }

  return LoadTexturePriv(texture_ptr, width, height, renderer, channels, data);
}

bool LoadTextureFromMemory(const void* inData, int len, SDL_Texture** texture_ptr, int* width, int* height, SDL_Renderer* renderer) {
  int channels = 0;
  unsigned char* data = stbi_load_from_memory(inData, len, width, height, &channels, 0);

  if (data == NULL) {
    fprintf(stderr, "Failed to load image: reason: %s\n", stbi_failure_reason());
    return false;
  }

  return LoadTexturePriv(texture_ptr, width, height, renderer, channels, data);
}