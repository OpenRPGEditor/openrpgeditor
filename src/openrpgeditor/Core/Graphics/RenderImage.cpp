#include "Core/Graphics/RenderImage.hpp"

#include "Core/Application.hpp"
#include "Core/Graphics/Texture.hpp"

#include <SDL.h>

RenderImage::RenderImage(const int width, const int height) : m_width(width), m_height(height) {
  m_renderImage = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width, height);
  SDL_SetTextureBlendMode(static_cast<SDL_Texture*>(m_renderImage), SDL_BLENDMODE_BLEND);
  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  auto* oldTarget = SDL_GetRenderTarget(renderer);
  SDL_SetRenderTarget(renderer, static_cast<SDL_Texture*>(m_renderImage));
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  SDL_SetRenderTarget(renderer, oldTarget);
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

void RenderImage::bind() {
  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  m_oldTarget = SDL_GetRenderTarget(renderer);
  SDL_SetRenderTarget(renderer, static_cast<SDL_Texture*>(m_renderImage));
  SDL_GetRenderDrawBlendMode(renderer, reinterpret_cast<SDL_BlendMode*>(&m_oldBlend));
  setCurrentCompositionMode();
}

void RenderImage::unbind() {
  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  SDL_SetRenderDrawBlendMode(renderer, static_cast<SDL_BlendMode>(m_oldBlend));
  SDL_SetRenderTarget(renderer, static_cast<SDL_Texture*>(m_oldTarget));
}

void RenderImage::eraseRect(const RectF& rect) {
  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  // bind();
  const SDL_FRect renderRect = {rect.x(), rect.y(), rect.width(), rect.height()};
  SDL_RenderFillRectF(renderer, &renderRect);
  // unbind();
}

void RenderImage::drawImage(const RectF& target, const Texture& image, const Rect& src) {
  auto* renderer = App::APP->getWindow()->getNativeRenderer();
  // bind();
  const SDL_Rect srcrect{src.x(), src.y(), src.width(), src.height()};
  const SDL_FRect destrect{target.x(), target.y(), target.width(), target.height()};
  SDL_RenderCopyF(renderer, static_cast<SDL_Texture*>(image.get()), &srcrect, &destrect);
  // unbind();
}
