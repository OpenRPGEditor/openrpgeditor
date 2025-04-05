﻿#include "Core/Graphics/CheckeredCompositeTexture.hpp"

#include "Core/Application.hpp"
#include <SDL3/SDL.h>

// static int roundUp(const int numToRound, const int multiple) {
//   if (multiple == 0)
//     return numToRound;
//
//   int remainder = abs(numToRound) % multiple;
//   if (remainder == 0)
//     return numToRound;
//
//   if (numToRound < 0)
//     return -(abs(numToRound) - remainder);
//   return numToRound + multiple - remainder;
// }

CheckeredCompositeTexture::CheckeredCompositeTexture()
: m_checker(32, 32) {}

ImTextureID CheckeredCompositeTexture::get() {
  if (m_isDirty) {
    update();
  }

  return reinterpret_cast<ImTextureID>(m_compositedTexture);
}

void CheckeredCompositeTexture::setSize(const int finalWidth, const int finalHeight) {
  m_finalSize = Point(finalWidth, finalHeight);
  m_isDirty = true;
}

void CheckeredCompositeTexture::setTexturesToComposite(const std::vector<CompositeTexture>& texturesToComposite) {
  m_texturesToComposite = texturesToComposite;
  m_isDirty = true;
}

void CheckeredCompositeTexture::update() {
  if (m_finalSize.isNull() || !m_isDirty) {
    return;
  }

  if (!m_compositedTexture) {
    m_compositedTexture = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_finalSize.x(), m_finalSize.y());
    SDL_SetTextureBlendMode(static_cast<SDL_Texture*>(m_compositedTexture), SDL_BLENDMODE_BLEND);
  }
  const int cols = std::ceil(static_cast<float>(m_finalSize.x()) / static_cast<float>(m_checker.width()));
  const int rows = std::ceil(static_cast<float>(m_finalSize.y()) / static_cast<float>(m_checker.height()));

  auto* oldTarget = SDL_GetRenderTarget(App::APP->getWindow()->getNativeRenderer());
  SDL_SetRenderTarget(App::APP->getWindow()->getNativeRenderer(), static_cast<SDL_Texture*>(m_compositedTexture));
  uint8_t r, g, b, a;
  SDL_GetRenderDrawColor(App::APP->getWindow()->getNativeRenderer(), &r, &g, &b, &a);
  SDL_SetRenderDrawColor(App::APP->getWindow()->getNativeRenderer(), 0, 0, 0, 0);
  SDL_RenderClear(App::APP->getWindow()->getNativeRenderer());

  {
    const SDL_FRect screct{0, 0, static_cast<float>(m_checker.width()), static_cast<float>(m_checker.height())};
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        const SDL_FRect dstrect{static_cast<float>(j * m_checker.width()), static_cast<float>(i * m_checker.height()), static_cast<float>(m_checker.width()), static_cast<float>(m_checker.height())};
        SDL_RenderTexture(App::APP->getWindow()->getNativeRenderer(), static_cast<SDL_Texture*>(m_checker.get()), &screct, &dstrect);
      }
    }
  }

  for (const auto& [texture, originalSize, offset, scaleToRect] : m_texturesToComposite) {
    const float aspect = static_cast<float>(originalSize.x()) / static_cast<float>(originalSize.y());
    int width = originalSize.x();
    int height = originalSize.y();

    if (scaleToRect) {
      width = m_finalSize.x();
      height = static_cast<int>(static_cast<float>(m_finalSize.x()) / aspect);
      if (height > m_finalSize.y()) {
        height = m_finalSize.y();
        width = static_cast<int>(static_cast<float>(m_finalSize.y()) * aspect);
      }
    }

    const SDL_FRect screct = {
        static_cast<float>(offset.x()),
        static_cast<float>(offset.y()),
        static_cast<float>(originalSize.x()),
        static_cast<float>(originalSize.y()),
    };
    const SDL_FRect dstrect = {
        std::abs(static_cast<float>(m_finalSize.x() / 2 - (width / 2))) + 1,
        std::abs(static_cast<float>(m_finalSize.y() / 2 - (height / 2))) + 1,
        static_cast<float>(width - 2),
        static_cast<float>(height - 2),
    };
    SDL_RenderTexture(App::APP->getWindow()->getNativeRenderer(), static_cast<SDL_Texture*>(texture.get()), &screct, &dstrect);
  }

  SDL_SetRenderTarget(App::APP->getWindow()->getNativeRenderer(), oldTarget);
  SDL_SetRenderDrawColor(App::APP->getWindow()->getNativeRenderer(), r, g, b, a);
  m_isDirty = false;
}
