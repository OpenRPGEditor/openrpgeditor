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
  m_lastTiles.clear();

  if (m_map == nullptr || m_tileset == nullptr) {
    SDL_DestroyTexture(m_lowerBitmap);
    SDL_DestroyTexture(m_upperBitmap);
    return;
  }

  for (int i = 0; i < m_tilesetTextures.size(); ++i) {
    if (!m_tileset->tilesetNames[i].empty()) {
      m_tilesetTextures[i] = ResourceManager::instance()->loadTilesetImage(m_tileset->tilesetNames[i]);
    }
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
}

void MapRenderer::update() {
  if (!m_map || !m_tileset) {
    return;
  }
}

void MapRenderer::drawTile(SDL_Texture* bitmap, int tileId, int dx, int dy) {
  if (isVisibleTile(tileId)) {
    if (isAutoTile(tileId)) {
      drawAutoTile(bitmap, tileId, dx, dy);
    } else {
      drawNormalTile(bitmap, tileId, dx, dy);
    }
  }
}

void MapRenderer::drawAutoTile(SDL_Texture* bitmap, int tileId, int dx, int dy) {}

void MapRenderer::drawNormalTile(SDL_Texture* bitmap, int tileId, int dx, int dy) {
  int setNumber = 0;

  if (isTileA5(tileId)) {
    setNumber = 4;
  } else {
    setNumber = 5 + static_cast<int>(floor(tileId / 256));
  }

  int w = m_tileWidth;
  int h = m_tileHeight;
  int sx = (static_cast<int>(floor(tileId / 128)) % 2 * 8 + tileId % 8) * w;
  int sy = (static_cast<int>(floor(tileId % 256 / 8)) % 16) * h;

  Texture& source = m_tilesetTextures[setNumber];

  SDL_Vertex verts[4] = {
      {
          {static_cast<float>(dx), static_cast<float>(dy)},
          {0xFF, 0xFF, 0xFF, 0xFF},
          {static_cast<float>(sx) / source.width(), static_cast<float>(sy) / source.height()},
      },
      {
          {static_cast<float>(dx + w), static_cast<float>(dy)},
          {0xFF, 0xFF, 0xFF, 0xFF},
          {static_cast<float>(sx + w) / source.width(), static_cast<float>(sy) / source.height()},
      },
      {
          {static_cast<float>(dx + w), static_cast<float>(dy + h)},
          {0xFF, 0xFF, 0xFF, 0xFF},
          {static_cast<float>(sx + w) / source.width(), static_cast<float>(sy + h) / source.height()},
      },
      {
          {static_cast<float>(dx), static_cast<float>(dy + h)},
          {0xFF, 0xFF, 0xFF, 0xFF},
          {static_cast<float>(sx) / source.width(), static_cast<float>(sy + h) / source.height()},
      }};
  const int indicies[4]{0, 1, 2, 3};

  SDL_RenderGeometry(App::APP->getWindow()->getNativeRenderer(), static_cast<SDL_Texture*>(source.get()), verts, 4,
                     indicies, 4);
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