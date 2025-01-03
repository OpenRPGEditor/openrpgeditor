#include "Core/Graphics/RenderImage.hpp"

#include "Core/Application.hpp"
#include "Core/Graphics/Texture.hpp"

#include <SDL3/SDL.h>

void RenderImage::createTexture() {
  if (m_renderImage[m_workBuffer]) {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(m_renderImage[m_workBuffer]));
  }
  m_renderImage[m_workBuffer] = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_pendingWidth, m_pendingHeight);
  SDL_SetTextureBlendMode(static_cast<SDL_Texture*>(m_renderImage[m_workBuffer]), SDL_BLENDMODE_BLEND);
  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  auto* oldTarget = SDL_GetRenderTarget(renderer);
  SDL_SetRenderTarget(renderer, static_cast<SDL_Texture*>(m_renderImage[m_workBuffer]));
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  SDL_SetRenderTarget(renderer, oldTarget);
}
RenderImage::RenderImage(const int width, const int height) : m_pendingWidth(width), m_pendingHeight(height) {
  for (int i = 0; i < 2; ++i) {
    m_width[i] = width;
    m_height[i] = height;
  }
}

void RenderImage::setSize(int width, int height) {
  m_pendingWidth = width;
  m_pendingHeight = height;
  m_bufferInvalid[0] = true;
  m_bufferInvalid[1] = true;
}

void RenderImage::setCurrentCompositionMode() const {
  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;
  switch (m_compositionMode) {
  case CompositionMode::SourceOver:
    blendMode =
        SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::DestinationOver:
    blendMode =
        SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::Clear:
    blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::Source:
    blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::Destination:
    blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::SourceIn:
    blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::DestinationIn:
    blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::SourceOut:
    blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDFACTOR_ZERO,
                                           SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::DestinationOut:
    blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                                           SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::SourceAtop:
    blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                                           SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::DestinationAtop:
    blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDFACTOR_SRC_ALPHA,
                                           SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::Xor:
    blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA,
                                           SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
    break;
  case CompositionMode::Plus:
    blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);
    break;
  }

  SDL_SetRenderDrawBlendMode(renderer, blendMode);
}

bool RenderImage::lock() {
  if (m_locked) {
    APP_WARN("Trying to lock an already locked image");
    return false;
  }

  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  // First we attempt to create the texture
  if (m_bufferInvalid[m_workBuffer]) {
    createTexture();
    m_width[m_workBuffer] = m_pendingWidth;
    m_height[m_workBuffer] = m_pendingHeight;
    m_bufferInvalid[m_workBuffer] = false;
  }

  // If we don't succeed bail so we don't do anything weird
  if (!m_renderImage[m_workBuffer]) {
    return false;
  }

  m_locked = true;
  m_oldTarget = SDL_GetRenderTarget(renderer);
  SDL_SetRenderTarget(renderer, static_cast<SDL_Texture*>(m_renderImage[m_workBuffer]));
  SDL_GetRenderDrawBlendMode(renderer, reinterpret_cast<SDL_BlendMode*>(&m_oldBlend));
  return true;
}

void RenderImage::unlock() {
  if (!m_locked) {
    APP_WARN("Trying to unlocked an already unlocked image");
    return;
  }

  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  if (SDL_GetRenderTarget(renderer) != m_renderImage[m_workBuffer]) {
    APP_ERROR("RenderImage::unlock() called when on non-matching render target!");
    return;
  }

  SDL_SetRenderDrawBlendMode(renderer, static_cast<SDL_BlendMode>(m_oldBlend));
  SDL_SetRenderTarget(renderer, static_cast<SDL_Texture*>(m_oldTarget));
  m_locked = false;
  std::swap(m_currentBuffer, m_workBuffer);
}

void RenderImage::eraseRect(const RectF& rect) const {
  if (!m_locked) {
    APP_WARN("Trying to erase on an unlocked image");
    return;
  }
  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  setCurrentCompositionMode();

  const SDL_FRect renderRect = {rect.x(), rect.y(), rect.width(), rect.height()};
  SDL_RenderFillRect(renderer, &renderRect);
}

void RenderImage::fillRect(const RectF& rect, const Color& color) const {
  if (!m_locked) {
    APP_WARN("Trying to erase on an unlocked image");
    return;
  }
  auto* renderer = App::APP->getWindow()->getNativeRenderer();

  if (SDL_GetRenderTarget(renderer) != m_renderImage[m_workBuffer]) {
    APP_ERROR("RenderImage::eraseRect() called when on non-matching render target!");
    return;
  }
  setCurrentCompositionMode();
  Uint8 r, g, b, a;
  SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
  const SDL_FRect renderRect = {rect.x(), rect.y(), rect.width(), rect.height()};
  Color::Rgb rgb = color.rgba();
  SDL_SetRenderDrawColor(renderer, Color::red(rgb), Color::green(rgb), Color::blue(rgb), Color::alpha(rgb));
  SDL_RenderFillRect(renderer, &renderRect);
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void RenderImage::drawImage(const RectF& target, const Texture& image, const Rect& src) const {
  if (!m_locked) {
    APP_WARN("Trying to draw on an unlocked image");
    return;
  }
  auto* renderer = App::APP->getWindow()->getNativeRenderer();

  if (SDL_GetRenderTarget(renderer) != m_renderImage[m_workBuffer]) {
    APP_ERROR("RenderImage::drawImage() called when on non-matching render target!");
    return;
  }
  setCurrentCompositionMode();
  const SDL_FRect srcRect{static_cast<float>(src.x()), static_cast<float>(src.y()), static_cast<float>(src.width()), static_cast<float>(src.height())};
  const SDL_FRect destRect{target.x(), target.y(), target.width(), target.height()};
  SDL_RenderTexture(renderer, static_cast<SDL_Texture*>(image.get()), &srcRect, &destRect);
}

void RenderImage::drawImageRaw(const RectF& target, void* image, const Rect& src) const {
  if (!m_locked) {
    APP_WARN("Trying to draw on an unlocked image");
    return;
  }
  auto* renderer = App::APP->getWindow()->getNativeRenderer();

  if (SDL_GetRenderTarget(renderer) != m_renderImage[m_workBuffer]) {
    APP_ERROR("RenderImage::drawImageRaw() called when on non-matching render target!");
    return;
  }
  setCurrentCompositionMode();
  const SDL_FRect srcRect{static_cast<float>(src.x()), static_cast<float>(src.y()), static_cast<float>(src.width()), static_cast<float>(src.height())};
  const SDL_FRect destRect{target.x(), target.y(), target.width(), target.height()};
  SDL_RenderTexture(renderer, static_cast<SDL_Texture*>(image), &srcRect, &destRect);
}