#include "Core/Tilemap/TilemapView.hpp"
#include "Core/ResourceManager.hpp"
#include "Database/Database.hpp"
#include "Database/TileHelper.hpp"

void TilemapView::setMap(const Map* map) {
  if (map == m_map) {
    return;
  }
  m_map = map;
  if (!m_map) {
    return;
  }
  setTileset(Database::instance()->tilesets->tileset(map->tilesetId()));
  createRenderImages();
  setDirtyRect(0, 0, m_map->width() + 1, m_map->height() + 1);
  m_frameDelay = 2;
}

void TilemapView::setTileset(const Tileset* tileset) {
  if (tileset == m_tileset) {
    return;
  }
  m_tileset = tileset;
  if (!m_tileset) {
    return;
  }
  for (int i = 0; i < 9; ++i) {
    if (m_tileset->tilesetName(i).empty()) {
      continue;
    }
    m_tilesetTextures[i] = ResourceManager::instance()->loadTilesetImage(m_tileset->tilesetName(i));
  }
}

void TilemapView::renderCharacter(EventPriority priority, const RectF& src, const RectF& dst, const Texture& tex) {
  const RenderImage& target = [priority, this]() -> RenderImage& {
    switch (priority) {
    case EventPriority::Below_character:
      return m_belowCharacterEvents;
    case EventPriority::Same_as_characters:
      return m_sameAsCharacterEvents;
    case EventPriority::Above_characters:
      return m_aboveCharacterEvents;
    }
    // Should never happen...
    return m_sameAsCharacterEvents;
  }();

  target.drawImage(src, tex, dst.toRect());
}
constexpr int tableEdgeVirtualId = 10000;

void TilemapView::render() {
  if (!m_map || !m_tileset || (m_frameDelay <= 0 && m_dirtyRect.isNull())) {
    return;
  }

  m_lowerTileIds.clear();
  m_upperTileIds.clear();
  for (int y = 0; y < m_map->height(); ++y) {
    for (int x = 0; x < m_map->width(); ++x) {
      if (!isTileDirty(x, y)) {
        continue;
      }
      paintTiles(0, 0, x, y);
    }
  }

  if (!m_lowerTileIds.empty()) {
    m_lowerTiles.lock();
    // First clear
    for (const auto& [x, y, _] : m_upperTileIds) {
      float dx = x;
      float dy = y;
      m_renderHelper.clearRect(m_lowerTiles, {dx, dy, realTileWidth(), realTileHeight()});
    }
    // Now render all the tiles
    for (const auto& [x, y, id] : m_lowerTileIds) {
      float dx = x;
      float dy = y;
      if (id < 0) {
        m_renderHelper.drawShadowTile(m_lowerTiles, {dx, dy, realTileWidth(), realTileHeight()}, std::abs(id));
      } else if (id >= tableEdgeVirtualId) {
        m_renderHelper.drawTile(m_lowerTiles, {dx, dy, realTileWidth(), realTileHeight()}, id - tableEdgeVirtualId, m_tilesetTextures, true);
      } else {
        m_renderHelper.drawTile(m_lowerTiles, {dx, dy, realTileWidth(), realTileHeight()}, id, m_tilesetTextures, false);
      }
    }
    m_lowerTiles.unlock();
  }
  if (!m_upperTileIds.empty()) {
    m_upperTiles.lock();
    // First clear
    for (const auto& [x, y, _] : m_upperTileIds) {
      float dx = x;
      float dy = y;
      m_renderHelper.clearRect(m_upperTiles, {dx, dy, realTileWidth(), realTileHeight()});
    }

    // Now render all the tiles
    for (const auto& [x, y, id] : m_upperTileIds) {
      float dx = x;
      float dy = y;
      m_renderHelper.drawTile(m_upperTiles, {dx, dy, realTileWidth(), realTileHeight()}, id, m_tilesetTextures, false);
    }
    m_upperTiles.unlock();
  }

  if (m_frameDelay > 0) {
    m_frameDelay--;
  } else {
    if (m_debugTexture.lock()) {
      m_renderHelper.clearRect(m_debugTexture, {0, 0, m_map->width() * realTileWidth(), m_map->height() * realTileHeight()});
      for (int y = 0; y < m_map->height(); ++y) {
        for (int x = 0; x < m_map->width(); ++x) {
          if (!isTileDirty(x, y)) {
            continue;
          }
          m_debugTexture.fillRect({static_cast<float>(x) * realTileWidth(), static_cast<float>(y) * realTileHeight(), realTileWidth(), realTileHeight()}, Color(0, 0, 0, 127));
        }
      }
      m_debugTexture.unlock();
    }
    clearDirtyRect();
  }
}

void TilemapView::paintTiles(const int startX, const int startY, const int x, const int y) {
  int mx = startX + x;
  int my = startY + y;
  float dx = x * realTileWidth();
  float dy = y * realTileHeight();

  int tileId0 = tileId(mx, my, 0);
  int tileId1 = tileId(mx, my, 1);
  int tileId2 = tileId(mx, my, 2);
  int tileId3 = tileId(mx, my, 3);
  int shadowBits = tileId(mx, my, 4);
  int upperTileId1 = tileId(mx, my - 1, 4);

  if (isHigherTile(tileId0)) {
    m_upperTileIds.emplace_back(dx, dy, tileId0);
  } else {
    m_lowerTileIds.emplace_back(dx, dy, tileId0);
  }

  if (isHigherTile(tileId1)) {
    m_upperTileIds.emplace_back(dx, dy, tileId1);
  } else {
    m_lowerTileIds.emplace_back(dx, dy, tileId1);
  }

  m_lowerTileIds.emplace_back(dx, dy, -shadowBits);

  if (isTableTile(upperTileId1) && !isTableTile(tileId1)) {
    if (!TileHelper::isShadowingTile(tileId0)) {
      m_lowerTileIds.emplace_back(dx, dy, tableEdgeVirtualId + upperTileId1);
    }
  }

  if (isOverpassPosition(mx, my)) {
    m_upperTileIds.emplace_back(dx, dy, tileId2);
    m_upperTileIds.emplace_back(dx, dy, tileId3);
  } else {
    if (isHigherTile(tileId2)) {
      m_upperTileIds.emplace_back(dx, dy, tileId2);
    } else {
      m_lowerTileIds.emplace_back(dx, dy, tileId2);
    }

    if (isHigherTile(tileId3)) {
      m_upperTileIds.emplace_back(dx, dy, tileId3);
    } else {
      m_lowerTileIds.emplace_back(dx, dy, tileId3);
    }
  }
}
int TilemapView::tileId(const int x, const int y, const int z) const {
  if (!m_map) {
    return 0;
  }
  return m_map->data()[(z * m_map->height() + y) * m_map->width() + x].value_or(0);
}

std::vector<int> TilemapView::layeredTiles(const int x, const int y) const {
  std::vector<int> ret;
  ret.reserve(4);

  for (int i = 0; i < 4; ++i) {
    ret.push_back(tileId(x, y, 3 - i));
  }

  return ret;
}
std::vector<std::optional<int>> TilemapView::tilesetFlags() const {
  if (m_tileset) {
    return m_tileset->flags();
  }
  return {};
}

std::vector<int> TilemapView::allTiles(const int x, const int y) const {
  if (!m_map) {
    return {};
  }

  std::vector<int> ret = layeredTiles(x, y);
  for (const auto eventsAt = m_map->eventsAt(x, y); const auto& event : eventsAt) {
    if (!event || !event->editor()) {
      continue;
    }

    const auto page = event->page(event->editor()->getSelectedPage());
    if (!page) {
      continue;
    }
    if (page->image().tileId() > 0) {
      ret.emplace_back(page->image().tileId());
    }
  }
  return ret;
}

bool TilemapView::checkPassage(const int x, const int y, const int bit) const {
  const auto& flags = tilesetFlags();
  const auto tiles = allTiles(x, y);

  for (int i = 0; i < tiles.size(); ++i) {
    const int flag = *flags[tiles[i]];
    if ((flag & 0x10) != 0) { // [*] No Effect
      continue;
    }
    if ((flag & bit) == 0) { // [o] Passable
      return true;
    }
    if ((flag & bit) == bit) { // [x] impassable
      return false;
    }
  }
  return false;
}

[[nodiscard]] bool TilemapView::isHigherTile(int tileId) const { return m_tileset->flag(tileId) & 0x10; }

[[nodiscard]] bool TilemapView::isTableTile(const int tileId) const { return TileHelper::isTileA2(tileId) && m_tileset->flag(tileId) & 0x80; }

[[nodiscard]] bool TilemapView::isOverpassPosition(const int mx, const int my) const { return false; }

void TilemapView::createRenderImages() {
  if (!m_map) {
    return;
  }
  const int realWidth = m_map->width() * realTileWidth();
  const int realHeight = m_map->height() * realTileHeight();
  m_lowerTiles = createRenderTexture(realWidth, realHeight);
  m_belowCharacterEvents = createRenderTexture(realWidth, realHeight);
  m_sameAsCharacterEvents = createRenderTexture(realWidth, realHeight);
  m_aboveCharacterEvents = createRenderTexture(realWidth, realHeight);
  m_upperTiles = createRenderTexture(realWidth, realHeight);
  m_debugTexture = createRenderTexture(realWidth, realHeight);
}