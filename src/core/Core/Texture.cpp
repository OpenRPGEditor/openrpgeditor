#include "Core/Texture.hpp"
#include "Core/Application.hpp"
#include "Core/image_loader.h"
#include "SDL2/SDL.h"

#include <iostream>

Texture::Texture(std::string_view filename) {
  SDL_Texture* tmp;
  LoadTextureFromFile(filename.data(), &tmp, &m_width, &m_height,
                      App::APP->getWindow()->getNativeRenderer());
  m_texture = tmp;
}