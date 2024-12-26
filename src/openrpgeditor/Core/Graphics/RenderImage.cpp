#include "Core/Graphics/RenderImage.hpp"

#include "Core/Application.hpp"
#include "Core/Graphics/Texture.hpp"

#include <SDL.h>

void RenderImage::createTexture() {
  if (m_renderImage) {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(m_renderImage));
  }
  m_renderImage = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
  SDL_SetTextureBlendMode(static_cast<SDL_Texture*>(m_renderImage), SDL_BLENDMODE_BLEND);
  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  auto* oldTarget = SDL_GetRenderTarget(renderer);
  SDL_SetRenderTarget(renderer, static_cast<SDL_Texture*>(m_renderImage));
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  SDL_SetRenderTarget(renderer, oldTarget);
}
RenderImage::RenderImage(const int width, const int height) : m_width(width), m_height(height) {}

void RenderImage::setSize(int width, int height) {
  m_width = width;
  m_height = height;
  if (m_renderImage) {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(m_renderImage));
    m_renderImage = nullptr;
  }
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

bool RenderImage::bind() {
  if (m_bound) {
    APP_WARN("Trying to bind an already bound image");
    return true;
  }

  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  if (SDL_GetRenderTarget(renderer) != nullptr) {
    APP_ERROR("RenderImage::bind() called when render target already set!");
  }

  // First we attempt to create the texture
  if (!m_renderImage) {
    createTexture();
  }
  // If we don't succeed bail so we don't do anything weird
  if (!m_renderImage) {
    return false;
  }

  m_bound = true;
  m_oldTarget = SDL_GetRenderTarget(renderer);
  SDL_SetRenderTarget(renderer, static_cast<SDL_Texture*>(m_renderImage));
  SDL_GetRenderDrawBlendMode(renderer, reinterpret_cast<SDL_BlendMode*>(&m_oldBlend));
  setCurrentCompositionMode();
  return true;
}

void RenderImage::unbind() {
  if (!m_bound) {
    APP_WARN("Trying to unbind an already unbound image");
    return;
  }

  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  if (SDL_GetRenderTarget(renderer) != m_renderImage) {
    APP_ERROR("RenderImage::unbind() called when on non-matching render target!");
    return;
  }

  SDL_SetRenderDrawBlendMode(renderer, static_cast<SDL_BlendMode>(m_oldBlend));
  SDL_SetRenderTarget(renderer, static_cast<SDL_Texture*>(m_oldTarget));
  m_bound = false;
}

void RenderImage::eraseRect(const RectF& rect) const {
  if (!m_bound) {
    APP_WARN("Trying to erase on an unbound image");
  }
  auto* renderer = App::APP->getWindow()->getNativeRenderer();

  if (SDL_GetRenderTarget(renderer) != m_renderImage) {
    APP_ERROR("RenderImage::eraseRect() called when on non-matching render target!");
    return;
  }

  const SDL_FRect renderRect = {rect.x(), rect.y(), rect.width(), rect.height()};
  SDL_RenderFillRectF(renderer, &renderRect);
}

void RenderImage::fillRect(const RectF& rect, const Color& color) const {
  if (!m_bound) {
    APP_WARN("Trying to erase on an unbound image");
  }
  auto* renderer = App::APP->getWindow()->getNativeRenderer();

  if (SDL_GetRenderTarget(renderer) != m_renderImage) {
    APP_ERROR("RenderImage::eraseRect() called when on non-matching render target!");
    return;
  }

  Uint8 r, g, b, a;
  SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
  const SDL_FRect renderRect = {rect.x(), rect.y(), rect.width(), rect.height()};
  Color::Rgb rgb = color.rgba();
  SDL_SetRenderDrawColor(renderer, Color::red(rgb), Color::green(rgb), Color::blue(rgb), Color::alpha(rgb));
  SDL_RenderFillRectF(renderer, &renderRect);
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void RenderImage::drawImage(const RectF& target, const Texture& image, const Rect& src) const {
  if (!m_bound) {
    APP_WARN("Trying to draw on an unbound image");
  }
  auto* renderer = App::APP->getWindow()->getNativeRenderer();

  if (SDL_GetRenderTarget(renderer) != m_renderImage) {
    APP_ERROR("RenderImage::drawImage() called when on non-matching render target!");
    return;
  }

  const SDL_Rect srcRect{src.x(), src.y(), src.width(), src.height()};
  const SDL_FRect destRect{target.x(), target.y(), target.width(), target.height()};
  SDL_RenderCopyF(renderer, static_cast<SDL_Texture*>(image.get()), &srcRect, &destRect);
}

void RenderImage::drawImageRaw(const RectF& target, void* image, const Rect& src) const {
  if (!m_bound) {
    APP_WARN("Trying to draw on an unbound image");
  }
  auto* renderer = App::APP->getWindow()->getNativeRenderer();

  if (SDL_GetRenderTarget(renderer) != m_renderImage) {
    APP_ERROR("RenderImage::drawImage() called when on non-matching render target!");
    return;
  }

  const SDL_Rect srcRect{src.x(), src.y(), src.width(), src.height()};
  const SDL_FRect destRect{target.x(), target.y(), target.width(), target.height()};
  SDL_RenderCopyF(renderer, static_cast<SDL_Texture*>(image), &srcRect, &destRect);
}
