#include "Core/Graphics/TileMarker.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/ResourceManager.hpp"

TileMarker::TileMarker(TileFlags flags, int pageIndex, int width, int height) {
  m_pageIndex = pageIndex;
  m_flags = flags;
  m_imageTexture = ResourceManager::instance()->loadTileMarkers(width, height);
  m_imageWidth = m_imageTexture.width() / kNumColumns;
  m_imageHeight = m_imageTexture.height() / kNumRows;
}
int TileMarker::imageWidth() const { return m_imageTexture.width(); }
int TileMarker::imageHeight() const { return m_imageTexture.height(); }
ImVec2 TileMarker::uv0() const {
  const auto u = static_cast<float>(m_markerType.x * m_imageWidth);
  const auto v = static_cast<float>(m_markerType.y * m_imageHeight);
  return {u / static_cast<float>(imageWidth()), v / static_cast<float>(imageHeight())};
}
ImVec2 TileMarker::uv1() const {
  const auto u = static_cast<float>(m_markerType.x * m_imageWidth) + static_cast<float>(m_imageWidth);
  const auto v = static_cast<float>(m_markerType.y * m_imageHeight) + static_cast<float>(m_imageWidth);
  return {u / static_cast<float>(imageWidth()), v / static_cast<float>(imageHeight())};
}
ImVec2 TileMarker::uvTileSize() const { return {1.0f / (m_imageTexture.width() / m_imageWidth), 1.0f / (m_imageTexture.width() / m_imageHeight)}; }
ImVec2 TileMarker::rectSize() const { return ImVec2(m_imageWidth, m_imageHeight); }
ImVec2 TileMarker::passableOffset(bool isPassable, bool hasHigherTile) const { return hasHigherTile ? ImVec2{0.f, 0.75f} : (isPassable ? ImVec2{0.f, 0.f} : ImVec2{0.25f, 0.f}); }
void TileMarker::setMarker(TileFlags flags) const {
  // m_markerType = flags;
}
