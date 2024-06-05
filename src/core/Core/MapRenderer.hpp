#pragma once

#include <vector>
#include <algorithm>

#include "Core/Texture.hpp"

#include "Database/Tilesets.hpp"

class Map;
class Tileset;
struct SDL_Texture;

class MapRenderer {
public:
  void setMap(const Map* map, const Tileset* tilest);

  void invalidate(int tileIndex) {
    if (tileIndex < 0 || tileIndex >= m_invalidTiles.size()) {
      return;
    }

    m_invalidTiles[tileIndex] = true;
  }

  bool isInvalid() {
    return std::any_of(m_invalidTiles.begin(), m_invalidTiles.end(), [](const bool v) { return v; });
  }
  void update(int mouseX, int mouseY);

  int tileId(int x, int y, int z) const;
  bool isOverworld() const { return m_tileset && m_tileset->mode == Tileset::Mode::World; }

  [[nodiscard]] std::vector<int> tilesetFlags() const {
    if (m_tileset) {
      return m_tileset->flags;
    }
    return {};
  }

  [[nodiscard]] std::vector<int> layeredTiles(const int x, const int y) const;
  [[nodiscard]] std::vector<int> allTiles(const int x, const int y) const;
  [[nodiscard]] int autoTileType(const int x, const int y, const int z) const;
  [[nodiscard]] bool checkPassage(const int x, const int y, const int bit) const;
  [[nodiscard]] bool isPassable(const int x, const int y, const int d) const;

private:
  void draw();
  const Map* m_map = nullptr;
  const Tileset* m_tileset = nullptr;
  std::array<Texture, 9> m_tilesetTextures;
  bool m_isValid = false;
  SDL_Texture* m_lowerTexture = nullptr;
  SDL_Texture* m_upperTexture = nullptr;
  std::vector<bool> m_invalidTiles;
};
