#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdbool.h>

#if _WIN32
#define STBI_WINDOWS_UTF8
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool LoadTextureFromFile(const char* filename, SDL_Texture** texture_ptr, int* width, int* height,
                         SDL_Renderer* renderer) {
  int channels;
  unsigned char* data = stbi_load(filename, width, height, &channels, 0);

  if (data == NULL) {
    fprintf(stderr, "Failed to load image: %s\n", stbi_failure_reason());
    return false;
  }

  SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)data, *width, *height, channels * 8, channels * *width,
                                                  0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

  if (surface == NULL) {
    fprintf(stderr, "Failed to create SDL surface: %s\n", SDL_GetError());
    return false;
  }

  *texture_ptr = SDL_CreateTextureFromSurface(renderer, surface);

  if ((*texture_ptr) == NULL) {
    fprintf(stderr, "Failed to create SDL texture: %s\n", SDL_GetError());
  }

  SDL_FreeSurface(surface);
  stbi_image_free(data);

  return true;
}
