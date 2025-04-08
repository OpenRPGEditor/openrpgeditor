#include "Core/Graphics/GameWindowBackground.hpp"

#include "Core/Application.hpp"

#include <SDL3/SDL.h>

static int roundUp(const int numToRound, const int multiple) {
  if (multiple == 0)
    return numToRound;

  int remainder = abs(numToRound) % multiple;
  if (remainder == 0)
    return numToRound;

  if (numToRound < 0)
    return -(abs(numToRound) - remainder);
  return numToRound + multiple - remainder;
}

void GameWindowBackground::update(const int r, const int g, const int b) {
  if (!ResourceManager::instance()) {
    return;
  }

  if (!m_renderTexture) {
    m_renderTexture = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    SDL_SetTextureBlendMode(static_cast<SDL_Texture*>(m_renderTexture), SDL_BLENDMODE_BLEND);
  }

  if (!m_resultTexture) {
    m_resultTexture = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, m_width, m_height);
    SDL_SetTextureBlendMode(static_cast<SDL_Texture*>(m_resultTexture), SDL_BLENDMODE_BLEND);
  }

  if (!m_windowTexture) {
    m_windowTexture = ResourceManager::instance()->loadSystemImage("Window");
  }

  if (!m_renderTexture || !m_windowTexture) {
    return;
  }

  auto oldRenderTarget = SDL_GetRenderTarget(App::APP->getWindow()->getNativeRenderer());
  SDL_SetRenderTarget(App::APP->getWindow()->getNativeRenderer(), static_cast<SDL_Texture*>(m_renderTexture));
  if (m_sizeChanged || !m_isInitialized) {
    SDL_SetRenderDrawColor(App::APP->getWindow()->getNativeRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(App::APP->getWindow()->getNativeRenderer());
    int cols = roundUp(m_width, 96) / 96;
    int rows = roundUp(m_height, 96) / 96;

    SDL_FRect screct{0, 0, 96, 96};
    SDL_FRect dstrect{0, 0, static_cast<float>(m_width), static_cast<float>(m_height)};
    SDL_RenderTexture(App::APP->getWindow()->getNativeRenderer(), static_cast<SDL_Texture*>(m_windowTexture.get()), &screct, &dstrect);

    screct = {0, 96, 96, 96};
    SDL_RenderTextureTiled(App::APP->getWindow()->getNativeRenderer(), static_cast<SDL_Texture*>(m_windowTexture.get()), &screct, 1.f, nullptr);
    SDL_RenderPresent(App::APP->getWindow()->getNativeRenderer());
    m_sizeChanged = false;
    m_isInitialized = true;
  }

  const SDL_Rect screct = {0, 0, m_width, m_height};
  if (auto surface = SDL_RenderReadPixels(App::APP->getWindow()->getNativeRenderer(), &screct)) {
    uint8_t* resultPixels;
    if (SDL_LockSurface(surface)) {
      uint8_t* sourcePixels = static_cast<uint8_t*>(surface->pixels);
      int pitch = 0;
      if (SDL_LockTexture(static_cast<SDL_Texture*>(m_resultTexture), nullptr, reinterpret_cast<void**>(&resultPixels), &pitch)) {
        for (int k = 0; k < m_width * m_height * 4; k += 4) {
          resultPixels[k + 3] = std::clamp(sourcePixels[k + 3] + 0, 0, 255);
          resultPixels[k + 0] = std::clamp(sourcePixels[k + 0] + r, 0, 255);
          resultPixels[k + 1] = std::clamp(sourcePixels[k + 1] + g, 0, 255);
          resultPixels[k + 2] = std::clamp(sourcePixels[k + 2] + b, 0, 255);
        }
        SDL_UnlockTexture(static_cast<SDL_Texture*>(m_resultTexture));
      }
      SDL_UnlockSurface(surface);
    }
  }

  SDL_SetRenderTarget(App::APP->getWindow()->getNativeRenderer(), oldRenderTarget);
}

void GameWindowBackground::setSize(const int width, const int height) {
  if (width != m_width || height != m_height) {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(m_resultTexture));
    SDL_DestroyTexture(static_cast<SDL_Texture*>(m_renderTexture));
    m_resultTexture = nullptr;
    m_renderTexture = nullptr;
    m_width = width;
    m_height = height;
    m_sizeChanged = true;
  }
}