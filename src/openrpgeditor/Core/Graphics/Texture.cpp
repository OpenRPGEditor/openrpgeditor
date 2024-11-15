#include "Core/Graphics/Texture.hpp"
#include "Core/Application.hpp"
#include "Core/Graphics/image_loader.h"
#include "SDL2/SDL.h"

#include <iostream>

Texture::Texture(std::string_view filename) {
  SDL_Texture* tmp = nullptr;
  if (LoadTextureFromFile(filename.data(), &tmp, &m_width, &m_height, App::APP->getWindow()->getNativeRenderer())) {
    m_texture = tmp;
  } else {
    m_texture = nullptr;
  }
}

 Texture::Texture(const char* data, int length, int width, int height) {
  SDL_Texture* tmp = nullptr;
  if (LoadTextureFromMemory(data, length, &tmp, &m_width, &m_height, App::APP->getWindow()->getNativeRenderer())) {
    m_texture = tmp;
  } else {
    m_texture = nullptr;
  }
}
