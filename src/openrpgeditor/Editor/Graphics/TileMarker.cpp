#include "Editor/Graphics/TileMarker.hpp"

#include "Editor/ResourceManager.hpp"

TileMarker::TileMarker(TileFlags flags, int pageIndex, int width, int height) {
  m_pageIndex = pageIndex;
  m_flags = flags;
  m_imageTexture = ResourceManager::instance()->loadTileMarkers(width, height);
  m_imageWidth = m_imageTexture.width() / kNumColumns;
  m_imageHeight = m_imageTexture.height() / kNumRows;
}
int TileMarker::imageWidth() const { return m_imageTexture.width(); }
int TileMarker::imageHeight() const { return m_imageTexture.height(); }
ImVec2 TileMarker::uv0(int markerIndex) const {
  const auto u = static_cast<float>((getMarkerUv(markerIndex).x) * m_imageWidth);
  const auto v = static_cast<float>((getMarkerUv(markerIndex).y) * m_imageHeight) + getMarkerUv(markerIndex).y;
  return {u / static_cast<float>(imageWidth()), v / static_cast<float>(imageHeight())};
}
ImVec2 TileMarker::uv1(int markerIndex) const {
  const auto u = static_cast<float>((getMarkerUv(markerIndex).x) * m_imageWidth) + (static_cast<float>(m_imageWidth));
  const auto v = static_cast<float>((getMarkerUv(markerIndex).y) * m_imageHeight) + (static_cast<float>(m_imageWidth));
  return {u / static_cast<float>(imageWidth()), v / static_cast<float>(imageHeight())};
}
ImVec2 TileMarker::uvTileSize() const { return {1.0f / (m_imageTexture.width() / m_imageWidth), 1.0f / (m_imageTexture.width() / m_imageHeight)}; }
ImVec2 TileMarker::rectSize() const { return ImVec2(m_imageWidth, m_imageHeight); }
ImVec2 TileMarker::passableOffset(bool isPassable, bool hasHigherTile) const { return hasHigherTile ? ImVec2{0.f, 0.75f} : (isPassable ? ImVec2{0.f, 0.f} : ImVec2{0.25f, 0.f}); }
ImVec2 TileMarker::getMarkerUv(int markerIndex) const {
  if (markerIndex > 15) {
    return {static_cast<float>(markerIndex - 16), 4};
  }
  if (markerIndex > 11) {
    return {static_cast<float>(markerIndex - 12), 3};
  }
  if (markerIndex > 7) {
    return {static_cast<float>(markerIndex - 8), 2};
  }
  if (markerIndex > 3) {
    return {static_cast<float>(markerIndex - 4), 1};
  }
  return {static_cast<float>(markerIndex), 0};
}
void TileMarker::setMarker(TileFlags flags) const {
  // m_markerType = flags;
}
