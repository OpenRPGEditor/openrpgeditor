#include "MapRenderer.hpp"

#include "SDL2/SDL.h"
#include "Core/Application.hpp"
#include "Core/ResourceManager.hpp"

#include <cmath>
#include "Database/Map.hpp"

constexpr std::array<std::array<std::array<int, 2>, 4>, 48> FloorAutoTileTableRaw{{
    {{{2, 4}, {1, 4}, {2, 3}, {1, 3}}}, {{{2, 0}, {1, 4}, {2, 3}, {1, 3}}}, {{{2, 4}, {3, 0}, {2, 3}, {1, 3}}},
    {{{2, 0}, {3, 0}, {2, 3}, {1, 3}}}, {{{2, 4}, {1, 4}, {2, 3}, {3, 1}}}, {{{2, 0}, {1, 4}, {2, 3}, {3, 1}}},
    {{{2, 4}, {3, 0}, {2, 3}, {3, 1}}}, {{{2, 0}, {3, 0}, {2, 3}, {3, 1}}}, {{{2, 4}, {1, 4}, {2, 1}, {1, 3}}},
    {{{2, 0}, {1, 4}, {2, 1}, {1, 3}}}, {{{2, 4}, {3, 0}, {2, 1}, {1, 3}}}, {{{2, 0}, {3, 0}, {2, 1}, {1, 3}}},
    {{{2, 4}, {1, 4}, {2, 1}, {3, 1}}}, {{{2, 0}, {1, 4}, {2, 1}, {3, 1}}}, {{{2, 4}, {3, 0}, {2, 1}, {3, 1}}},
    {{{2, 0}, {3, 0}, {2, 1}, {3, 1}}}, {{{0, 4}, {1, 4}, {0, 3}, {1, 3}}}, {{{0, 4}, {3, 0}, {0, 3}, {1, 3}}},
    {{{0, 4}, {1, 4}, {0, 3}, {3, 1}}}, {{{0, 4}, {3, 0}, {0, 3}, {3, 1}}}, {{{2, 2}, {1, 2}, {2, 3}, {1, 3}}},
    {{{2, 2}, {1, 2}, {2, 3}, {3, 1}}}, {{{2, 2}, {1, 2}, {2, 1}, {1, 3}}}, {{{2, 2}, {1, 2}, {2, 1}, {3, 1}}},
    {{{2, 4}, {3, 4}, {2, 3}, {3, 3}}}, {{{2, 4}, {3, 4}, {2, 1}, {3, 3}}}, {{{2, 0}, {3, 4}, {2, 3}, {3, 3}}},
    {{{2, 0}, {3, 4}, {2, 1}, {3, 3}}}, {{{2, 4}, {1, 4}, {2, 5}, {1, 5}}}, {{{2, 0}, {1, 4}, {2, 5}, {1, 5}}},
    {{{2, 4}, {3, 0}, {2, 5}, {1, 5}}}, {{{2, 0}, {3, 0}, {2, 5}, {1, 5}}}, {{{0, 4}, {3, 4}, {0, 3}, {3, 3}}},
    {{{2, 2}, {1, 2}, {2, 5}, {1, 5}}}, {{{0, 2}, {1, 2}, {0, 3}, {1, 3}}}, {{{0, 2}, {1, 2}, {0, 3}, {3, 1}}},
    {{{2, 2}, {3, 2}, {2, 3}, {3, 3}}}, {{{2, 2}, {3, 2}, {2, 1}, {3, 3}}}, {{{2, 4}, {3, 4}, {2, 5}, {3, 5}}},
    {{{2, 0}, {3, 4}, {2, 5}, {3, 5}}}, {{{0, 4}, {1, 4}, {0, 5}, {1, 5}}}, {{{0, 4}, {3, 0}, {0, 5}, {1, 5}}},
    {{{0, 2}, {3, 2}, {0, 3}, {3, 3}}}, {{{0, 2}, {1, 2}, {0, 5}, {1, 5}}}, {{{0, 4}, {3, 4}, {0, 5}, {3, 5}}},
    {{{2, 2}, {3, 2}, {2, 5}, {3, 5}}}, {{{0, 2}, {3, 2}, {0, 5}, {3, 5}}}, {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
}};

constexpr std::array<std::array<std::array<int, 2>, 4>, 16> WallAutoTileTableRaw{{{{{2, 2}, {1, 2}, {2, 1}, {1, 1}}},
                                                                                  {{{0, 2}, {1, 2}, {0, 1}, {1, 1}}},
                                                                                  {{{2, 0}, {1, 0}, {2, 1}, {1, 1}}},
                                                                                  {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
                                                                                  {{{2, 2}, {3, 2}, {2, 1}, {3, 1}}},
                                                                                  {{{0, 2}, {3, 2}, {0, 1}, {3, 1}}},
                                                                                  {{{2, 0}, {3, 0}, {2, 1}, {3, 1}}},
                                                                                  {{{0, 0}, {3, 0}, {0, 1}, {3, 1}}},
                                                                                  {{{2, 2}, {1, 2}, {2, 3}, {1, 3}}},
                                                                                  {{{0, 2}, {1, 2}, {0, 3}, {1, 3}}},
                                                                                  {{{2, 0}, {1, 0}, {2, 3}, {1, 3}}},
                                                                                  {{{0, 0}, {1, 0}, {0, 3}, {1, 3}}},
                                                                                  {{{2, 2}, {3, 2}, {2, 3}, {3, 3}}},
                                                                                  {{{0, 2}, {3, 2}, {0, 3}, {3, 3}}},
                                                                                  {{{2, 0}, {3, 0}, {2, 3}, {3, 3}}},
                                                                                  {{{0, 0}, {3, 0}, {0, 3}, {3, 3}}}}};

constexpr std::array<std::array<std::array<int, 2>, 4>, 4> WaterfallAutoTileTableRaw{{
    {{{2, 0}, {1, 0}, {2, 1}, {1, 1}}},
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
    {{{2, 0}, {3, 0}, {2, 1}, {3, 1}}},
    {{{0, 0}, {3, 0}, {0, 1}, {3, 1}}},
}};

static std::vector<std::array<std::array<int, 2>, 4>> FloorTileTable;
static std::vector<std::array<std::array<int, 2>, 4>> WallTileTable;
static std::vector<std::array<std::array<int, 2>, 4>> WaterfallTileTable;

static bool tileTablesInitialized = false;
MapRenderer::MapRenderer() {
#if 1
  if (!tileTablesInitialized) {
    for (auto& v : FloorAutoTileTableRaw) {
      FloorTileTable.push_back(v);
    }
    for (auto& v : WallAutoTileTableRaw) {
      WallTileTable.push_back(v);
    }

    for (auto& v : WaterfallAutoTileTableRaw) {
      WaterfallTileTable.push_back(v);
    }

    tileTablesInitialized = true;
  }
#endif
}

void MapRenderer::setMap(const Map* map, const Tileset* tileset, int tileWidth, int tileHeight) {
  m_map = map;
  m_tileset = tileset;
  m_tileWidth = tileWidth;
  m_tileHeight = tileHeight;
  m_lastTiles.clear();
  if (m_map && m_tileset) {
    for (int i = 0; i < m_tilesetTextures.size(); ++i) {
      if (!m_tileset->tilesetNames[i].empty()) {
        m_tilesetTextures[i] = ResourceManager::instance()->loadTilesetImage(m_tileset->tilesetNames[i]);
      }
    }
  }

#if 0
  if (m_map == nullptr || m_tileset == nullptr) {
    SDL_DestroyTexture(m_lowerBitmap);
    SDL_DestroyTexture(m_upperBitmap);
    return;
  }


  if (m_lowerBitmap) {
    SDL_DestroyTexture(m_lowerBitmap);
  }

  if (m_upperBitmap) {
    SDL_DestroyTexture(m_upperBitmap);
  }
  m_lowerBitmap = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET, map->width * 48, map->height * 48);
  SDL_SetTextureBlendMode(m_lowerBitmap, SDL_BLENDMODE_BLEND);

  m_upperBitmap = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET, map->width * 48, map->height * 48);
  SDL_SetTextureBlendMode(m_upperBitmap, SDL_BLENDMODE_BLEND);
#endif
}

void MapRenderer::update() {
  if (!m_map || !m_tileset) {
    return;
  }
}

void MapRenderer::getTileRect(std::vector<TileRect>& layer, int tileId, int dx, int dy) {
  if (isVisibleTile(tileId)) {
    if (isAutoTile(tileId)) {
      drawAutoTile(layer, tileId, dx, dy);
    } else {
      drawNormalTile(layer, tileId, dx, dy);
    }
  }
}

void MapRenderer::drawTile(std::vector<TileRect>& layer, int tileId, int dx, int dy) {
  if (isVisibleTile(tileId)) {
    if (isAutoTile(tileId)) {
      drawAutoTile(layer, tileId, dx, dy);
    } else {
      drawNormalTile(layer, tileId, dx, dy);
    }
  }
}

void MapRenderer::drawAutoTile(std::vector<TileRect>& layer, int tileId, int dx, int dy) {
  auto autoTileTable = FloorTileTable;
  int kind = getAutoTileKind(tileId);
  int shape = getAutoTileShape(tileId);
  int tx = kind % 8;
  int ty = floor(kind / 8);
  int bx = 0;
  int by = 0;
  int setNumber = 0;
  bool isTable = false;
  int animX = 0;
  int animY = 0;

  if (isTileA1(tileId)) {
    setNumber = 0;
    if (kind == 0) {
      animX = 2;
      bx = 0;
    } else if (kind == 1) {
      animX = 2;
      by = 3;
    } else if (kind == 2) {
      bx = 6;
      by = 0;
    } else if (kind == 3) {
      bx = 6;
      by = 3;
    } else {
      bx = floor(tx / 4) * 8;
      by = ty * 6 + static_cast<int>(floor(tx / 2)) % 2 * 3;
      if ((kind % 2) == 0) {
        animX = 2;
      } else {
        bx += 6;
        autoTileTable = WaterfallTileTable;
        animY = 1;
      }
    }
  } else if (isTileA2(tileId)) {
    setNumber = 1;
    bx = tx * 2;
    by = (ty - 2) * 3;
    isTable = isTableTile(tileId);
  } else if (isTileA3(tileId)) {
    setNumber = 2;
    bx = tx * 2;
    by = (ty - 6) * 2;
    autoTileTable = WallTileTable;
  } else if (isTileA4(tileId)) {
    setNumber = 3;
    bx = tx * 2;
    by = floor((ty - 10) * 2.5 + (ty % 2 == 1 ? 0.5f : 0));
    if ((ty % 2) == 1) {
      autoTileTable = WallTileTable;
    }
  }

  const auto& table = autoTileTable[shape];
  int w1 = m_tileWidth / 2;
  int h1 = m_tileHeight / 2;

  for (int i = 0; i < 4; i++) {
    int qsx = table[i][0];
    int qsy = table[i][1];

    int sx1 = (bx * 2 + qsx) * w1;
    int sy1 = (by * 2 + qsy) * h1;
    int dx1 = dx + (i % 2) * w1;
    int dy1 = dy + static_cast<int>(floor(i / 2)) * h1;
    if (isTable && (qsy == 1 || qsy == 5)) {
      int qsx2 = qsx;
      int qsy2 = 3;
      if (qsy == 1) {
        //          static const int tbl[4] {0, 3, 2, 1};
        //          qsx2 = tbl[qsx];
        qsx2 = (4 - qsx) % 4;
      }

      int sx2 = (bx * 2 + qsx2) * w1;
      int sy2 = (by * 2 + qsy2) * h1;
      layer.emplace_back(tileId, setNumber, sx2, sy2, dx1, dy1, w1, h1, animX, animY);
      dy1 += h1 / 2;
      layer.emplace_back(tileId, setNumber, sx1, sy1, dx1, dy1 + h1 / 2, w1, h1 / 2, animX, animY);
    } else {
      layer.emplace_back(tileId, setNumber, sx1, sy1, dx1, dy1, w1, h1, animX, animY);
    }
  }
}

void MapRenderer::drawNormalTile(std::vector<TileRect>& layer, int tileId, int dx, int dy) const {
  int setNumber = 0;

  if (isTileA5(tileId)) {
    setNumber = 4;
  } else {
    setNumber = 5 + floor(tileId / 256);
  }

  int w = m_tileWidth;
  int h = m_tileHeight;
  float sx = (fmod(floor(tileId / 256), 2) * 8 + (tileId % 8)) * w;
  float sy = fmod(floor(tileId % 256 / 8), 16) * h;

  layer.emplace_back(tileId, setNumber, sx, sy, dx, dy, w, h);
}

void MapRenderer::beginBlit(SDL_Texture* bitmap) {
  SDL_Renderer* renderer = App::APP->getWindow()->getNativeRenderer();
  m_oldTarget = SDL_GetRenderTarget(renderer);
  SDL_SetRenderTarget(renderer, bitmap);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
}

void MapRenderer::blitImage(SDL_Texture* source, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh) {

  SDL_Renderer* renderer = App::APP->getWindow()->getNativeRenderer();
  SDL_Rect srect = {sx, sy, sw, sh};
  SDL_Rect drect{dx, dy, dw, dh};
  SDL_RenderCopy(renderer, source, &srect, &drect);
}

void MapRenderer::endBlit() {
  SDL_SetRenderTarget(App::APP->getWindow()->getNativeRenderer(), m_oldTarget);
  m_oldTarget = nullptr;
}

void MapRenderer::clearRect(SDL_Texture* clr, int x, int y, int w, int h) {
  SDL_Renderer* renderer = App::APP->getWindow()->getNativeRenderer();
  SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
  SDL_SetRenderTarget(renderer, clr);
  SDL_Rect rect{x, y, w, h};
  SDL_RenderFillRect(renderer, &rect);
  SDL_SetRenderTarget(renderer, oldTarget);
}

int MapRenderer::tileId(const int x, const int y, const int z) const {
  if (!m_map) {
    return 0;
  }

  int idx = (z * m_map->height + y) * m_map->width + x;
  if (idx < m_map->data.size()) {
    return m_map->data[idx];
  }

  return 0;
}

std::vector<int> MapRenderer::layeredTiles(const int x, const int y) const {
  std::vector<int> ret;
  ret.reserve(4);

  for (int i = 0; i < 4; ++i) {
    ret.push_back(tileId(x, y, 3 - i));
  }

  return ret;
}

std::vector<int> MapRenderer::allTiles(const int x, const int y) const {
  // TODO: Add event tiles?
  return layeredTiles(x, y);
}

int MapRenderer::autoTileType(const int x, const int y, const int z) const {
  int tile = tileId(x, y, z);
  return tile >= 2048 ? static_cast<int>(std::floor((tile - 2048) / 48)) : -1;
}

bool MapRenderer::checkPassage(const int x, const int y, const int bit) const {
  const auto& flags = tilesetFlags();
  const auto tiles = allTiles(x, y);

  for (int i = 0; i < tiles.size(); ++i) {
    const int flag = flags[tiles[i]];
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

bool MapRenderer::isPassable(const int x, const int y, const int d) const {
  return checkPassage(x, y, (1 << (d / 2 - 1)) & 0x0f);
}