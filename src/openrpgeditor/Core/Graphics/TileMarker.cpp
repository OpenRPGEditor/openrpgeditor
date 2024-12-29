#include "Core/Graphics/TileMarker.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/ResourceManager.hpp"

TileMarker::TileMarker(int markerType, int width, int height) {
  m_markerType = markerType;
  m_imageTexture = ResourceManager::instance()->loadTileMarkers(width, height);
}
int TileMarker::imageWidth() const { return m_imageTexture.width(); }
int TileMarker::imageHeight() const { return m_imageTexture.height(); }
ImVec2 TileMarker::uv0() const { return {m_markerType * uvTileSize().x, m_markerType * uvTileSize().y}; }
ImVec2 TileMarker::uv1() const { return {(m_markerType + 1) * uvTileSize().x, (m_markerType + 1) * uvTileSize().y}; }
ImVec2 TileMarker::uvTileSize() const { return {1.0f / (m_imageTexture.width() / m_imageWidth), 1.0f / (m_imageTexture.width() / m_imageHeight)}; }
ImVec2 TileMarker::rectSize() const { return ImVec2(16, 16) * App::DPIHandler::get_ui_scale(); }