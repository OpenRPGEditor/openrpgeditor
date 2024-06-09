#include "Core/Texture.hpp"
#include "Core/Application.hpp"
#include "Core/image_loader.h"
#include "SDL2/SDL.h"

#include <iostream>

static SDL_Texture* errorTex = nullptr;
Texture::Texture(std::string_view filename) {
  // if (errorTex == nullptr) {
  //   uint8_t* data = (uint8_t*)malloc(128 * 128 * 4);
  //   for (int y = 0; y < 128 * 4; y += 4) {
  //     for (int x = 0; x < 128 * 4; x += 4) {
  //       *(data + ((y * 128) + x) + 0) = 255;
  //       *(data + ((y * 128) + x) + 1) = 255;
  //       *(data + ((y * 128) + x) + 2) = 255;
  //       *(data + ((y * 128) + x) + 3) = 255;
  //     }
  //   }
  //   SDL_Renderer* renderer = App::APP->getWindow()->getNativeRenderer();
  //
  //   SDL_Surface* surface =
  //       SDL_CreateRGBSurfaceWithFormatFrom((void*)data, 128, 128, 32, 4 * 128, SDL_PIXELFORMAT_ARGB8888);
  //
  //   errorTex = SDL_CreateTextureFromSurface(renderer, surface);
  //   SDL_FreeSurface(surface);
  //   free(data);
  // }

  SDL_Texture* tmp = nullptr;
  if (LoadTextureFromFile(filename.data(), &tmp, &m_width, &m_height, App::APP->getWindow()->getNativeRenderer())) {
    m_texture = tmp;
  } else {
    m_texture = nullptr;
  }
}