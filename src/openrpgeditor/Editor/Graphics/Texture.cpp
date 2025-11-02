#include "Editor/Graphics/Texture.hpp"
#include "Editor/Application.hpp"
#include "Editor/Graphics/image_loader.h"
#include "SDL3/SDL.h"

#include <iostream>

Texture::Texture(std::string_view filename) {
  SDL_Texture* tmp = nullptr;
  if (LoadTextureFromFile(filename.data(), &tmp, &m_width, &m_height, App::APP->getWindow()->getNativeRenderer())) {
    m_texture = tmp;
    SDL_SetTextureScaleMode(static_cast<SDL_Texture*>(m_texture), SDL_SCALEMODE_NEAREST);
  } else {
    m_texture = nullptr;
  }
}

// Texture::Texture(const char* data, int length, int width, int height) {
//   SDL_Texture* tmp = nullptr;
//   if (LoadTextureFromMemory(data, length, &tmp, &m_width, &m_height, App::APP->getWindow()->getNativeRenderer())) {
//     m_texture = tmp;
//   } else {
//     m_texture = nullptr;
//   }
// }

void Texture::setFilter(FilterType filterType) {
  switch (filterType) {
  case Nearest:
    SDL_SetTextureScaleMode(static_cast<SDL_Texture*>(m_texture), SDL_SCALEMODE_NEAREST);
    break;
  case Linear:
    SDL_SetTextureScaleMode(static_cast<SDL_Texture*>(m_texture), SDL_SCALEMODE_LINEAR);
    break;
  }
}
