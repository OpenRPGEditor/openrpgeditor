#include "Core/Graphics/ParallaxTexture.hpp"

#include "Core/Application.hpp"
#include "Core/ResourceManager.hpp"
#include "Database/Database.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

// TODO: When the new renderer class is implemented replace direct ImGui call with the appropriate delta call

#include <imgui.h>

void ParallaxTexture::setTexture(const std::string_view path) {
  m_texture = ResourceManager::instance()->loadParallaxImage(path);
  update();
}

int ParallaxTexture::pixelWidth() const { return m_size.width() * Database::instance()->system->tileSize(); }
int ParallaxTexture::pixelHeight() const { return m_size.height() * Database::instance()->system->tileSize(); }

void ParallaxTexture::update() const {
  if (!m_texture || (!m_renderTexture && m_workDone)) {
    return;
  }

  if (!m_renderTexture) {
    m_renderTexture = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, pixelWidth(), pixelHeight());
  }

  if (m_updateScrolling) {
    m_frameTime += ImGui::GetIO().DeltaTime;
    if (m_loopX) {
      m_scrollPosition.xr() += m_scroll.x() * ImGui::GetIO().DeltaTime;
    }
    if (m_loopY) {
      m_scrollPosition.yr() += m_scroll.y() * ImGui::GetIO().DeltaTime;
    }

    if (m_scrollPosition.x() < 0) {
      m_scrollPosition.xr() = width();
    } else if (m_scrollPosition.x() > width()) {
      m_scrollPosition.xr() = 0;
    }

    if (m_scrollPosition.y() < 0) {
      m_scrollPosition.yr() = height();
    } else if (m_scrollPosition.y() > height()) {
      m_scrollPosition.yr() = 0;
    }

    if (m_frameTime >= 1.f / 60.f) {
      m_workDone = false;
      m_frameTime = 0.f;
    }
  } else {
    m_frameTime = 0.f;
  }

  if (m_workDone) {
    return;
  }
  SDL_Texture* target = SDL_GetRenderTarget(App::APP->getWindow()->getNativeRenderer());
  SDL_SetRenderTarget(App::APP->getWindow()->getNativeRenderer(), static_cast<SDL_Texture*>(m_renderTexture));

  if (m_updateScrolling && (m_loopX || m_loopY)) {
    const float sx = m_scrollPosition.x() * Database::instance()->system->tileSize();
    const float sy = m_scrollPosition.y() * Database::instance()->system->tileSize();
    for (float y = -pixelHeight(); y <= pixelHeight(); y += m_texture.height()) {
      for (float x = -pixelWidth(); x <= pixelWidth(); x += m_texture.width()) {
        SDL_FRect dst{x + sx, y + sy, static_cast<float>(m_texture.width()), static_cast<float>(m_texture.height())};
        SDL_RenderTexture(App::APP->getWindow()->getNativeRenderer(), static_cast<SDL_Texture*>(m_texture.get()), nullptr, &dst);
      }
    }
  } else {
    const SDL_FRect dst{0, 0, static_cast<float>(pixelWidth()), static_cast<float>(pixelHeight())};
    SDL_RenderTextureTiled(App::APP->getWindow()->getNativeRenderer(), static_cast<SDL_Texture*>(m_texture.get()), nullptr, 1.f, &dst);
  }

  SDL_SetRenderTarget(App::APP->getWindow()->getNativeRenderer(), target);
  m_workDone = true;
}
