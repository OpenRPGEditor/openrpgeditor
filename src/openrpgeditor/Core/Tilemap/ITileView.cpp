#include "Core/Tilemap/ITileView.hpp"

#include "Core/Application.hpp"
#include "Core/Graphics/Texture.hpp"

RenderImage ITileView::createRenderTexture(const int width, const int height) { return {width, height, false}; }

void ITileView::clearRect(RenderImage& painter, const RectF& rect) {
  const auto oldComposition = painter.compositionMode();
  painter.setCompositionMode(RenderImage::CompositionMode::Clear);
  painter.eraseRect(rect);
  painter.setCompositionMode(oldComposition);
}
