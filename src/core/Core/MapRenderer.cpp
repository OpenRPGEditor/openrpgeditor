#include "Core/MapRenderer.hpp"

#include "SDL2/SDL.h"
#include "Core/Application.hpp"
#include "Core/ResourceManager.hpp"

#include <cmath>
#include "Database/Map.hpp"

void MapRenderer::setMap(const Map* map, const Tileset* tileset, int tileWidth, int tileHeight) {
  m_map = map;
  m_tileset = tileset;
  m_tileWidth = tileWidth;
  m_tileHeight = tileHeight;

  if (m_map == nullptr || m_tileset == nullptr) {
    SDL_DestroyTexture(m_lowerTexture);
    SDL_DestroyTexture(m_upperTexture);
    return;
  }

  for (int i = 0; i < m_tilesetTextures.size(); ++i) {
    if (!m_tileset->tilesetNames[i].empty()) {
      m_tilesetTextures[i] = ResourceManager::instance()->loadTilesetImage(m_tileset->tilesetNames[i]);
    }
  }
  m_invalidTiles.resize(m_map->width * m_map->height);
  std::fill(m_invalidTiles.begin(), m_invalidTiles.end(), true);
  if (m_lowerTexture) {
    SDL_DestroyTexture(m_lowerTexture);
  }

  if (m_upperTexture) {
    SDL_DestroyTexture(m_upperTexture);
  }
  m_lowerTexture = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_TARGET, map->width * 48, map->height * 48);
  SDL_SetTextureBlendMode(m_lowerTexture, SDL_BLENDMODE_BLEND);

  m_upperTexture = SDL_CreateTexture(App::APP->getWindow()->getNativeRenderer(), SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_TARGET, map->width * 48, map->height * 48);
  SDL_SetTextureBlendMode(m_upperTexture, SDL_BLENDMODE_BLEND);
}

void MapRenderer::update(int mouseX, int mouseY) {
  draw();
}

int MapRenderer::tileId(const int x, const int y, const int z) const {

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

std::vector<int> MapRenderer::allTiles(const int x, const int y) const { return {}; }

int MapRenderer::autoTileType(const int x, const int y, const int z) const {
  int tile = tileId(x, y, z);
  return tile >= 2048 ? static_cast<int>(std::floor((tile - 2048) / 48)) : -1;
}

bool MapRenderer::checkPassage(const int x, const int y, const int bit) const { return false; }
bool MapRenderer::isPassable(const int x, const int y, const int d) const {
  return checkPassage(x, y, (1 << (d / 2 - 1)) & 0x0f);
}

void MapRenderer::draw() {
  //SDL_Texture* oldTarget = SDL_GetRenderTarget(App::APP->getWindow()->getNativeRenderer());
  //   SDL_SetRenderTarget(App::APP->getWindow()->getNativeRenderer(), m_lowerTexture);
  //   SDL_SetRenderDrawColor(App::APP->getWindow()->getNativeRenderer(), 255, 0, 255, 255);
  //   SDL_RenderClear(App::APP->getWindow()->getNativeRenderer());
  //   int tileCellX = (tileIndex / (m_testTexture.width() / m_tileWidth)) * tileWidth;
  //   int tileCellY = (tileIndex % (m_testTexture.width() / m_tileWidth)) * tileHeight;
  //   SDL_Rect tileRect{x, y, tileWidth, tileHeight};
  //   SDL_Rect tileSrcRect{tileCellX, tileCellY, tileWidth, tileHeight};
  //
  //
  //
  //   SDL_RenderCopy(App::APP->getWindow()->getNativeRenderer(), static_cast<SDL_Texture*>(m_testTexture.get()),
  //                  &tileSrcRect, &tileRect);
  //
  // SDL_SetRenderTarget(App::APP->getWindow()->getNativeRenderer(), oldTarget);
}
