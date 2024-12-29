#include "Core/Graphics/TileMarker.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/ResourceManager.hpp"

TileMarker::TileMarker(int markerType, int width, int height) {
  m_markerType = markerType;
  m_imageTexture = ResourceManager::instance()->loadTileMarkers(width, height);
  m_imageWidth = m_imageTexture.width() / kNumColumns;
  m_imageHeight = m_imageTexture.height() / kNumRows;
}
int TileMarker::imageWidth() const { return m_imageTexture.width(); }
int TileMarker::imageHeight() const { return m_imageTexture.height(); }
ImVec2 TileMarker::uv0() const {
  const auto u = static_cast<float>(m_markerType * m_imageWidth);
  const auto v = static_cast<float>(m_markerType * m_imageHeight);
  return {u / static_cast<float>(imageWidth()), v / static_cast<float>(imageHeight())};
}
ImVec2 TileMarker::uv1() const {
  const auto u = static_cast<float>(m_markerType * m_imageWidth) + static_cast<float>(m_imageWidth);
  const auto v = static_cast<float>(m_markerType * m_imageHeight) + static_cast<float>(m_imageWidth);
  return {u / static_cast<float>(imageWidth()), v / static_cast<float>(imageHeight())};
}
ImVec2 TileMarker::uvTileSize() const { return {1.0f / (m_imageTexture.width() / m_imageWidth), 1.0f / (m_imageTexture.width() / m_imageHeight)}; }
ImVec2 TileMarker::rectSize() const { return ImVec2(m_imageWidth, m_imageHeight); }