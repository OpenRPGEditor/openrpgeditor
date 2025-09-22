#include "Core/Tilemap/ITileView.hpp"

#include "Core/Application.hpp"
#include "Core/Graphics/Texture.hpp"

RenderImage ITileView::createRenderTexture(const int width, const int height) {
  RenderImage ri{width, height, false};
  ri.setFilterMode(RenderImage::FilterMode::Nearest);
  return ri;
}

void ITileView::clearRect(RenderImage& painter, const RectF& rect) {
  const auto oldComposition = painter.compositionMode();
  painter.setCompositionMode(RenderImage::CompositionMode::Clear);
  painter.eraseRect(rect);
  painter.setCompositionMode(oldComposition);
}
