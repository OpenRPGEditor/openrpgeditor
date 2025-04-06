#include "Core/Graphics/CharacterSheet.hpp"

#include "Core/ResourceManager.hpp"

#include "Database/Database.hpp"

CharacterSheet::CharacterSheet(std::string_view sheetName, bool isTileId, int tileId)
  : m_characterName(sheetName) {
  if (isTileId && tileId >= 0) {
    m_isTileId = isTileId;
    const int setId = 5 + floor(tileId / 256);
    const Map *map = nullptr;
    if (Database::instance()->mapInfos.currentMap() && Database::instance()->mapInfos.currentMap()->map()) {
      map = Database::instance()->mapInfos.currentMap()->map();
    }
    const Tileset *tileset = nullptr;
    if (map) {
      tileset = Database::instance()->tilesets.tileset(map->tilesetId());
    }
    if (tileset) {
      sheetName = tileset->tilesetName(setId);
    }
    m_sheetTexture = ResourceManager::instance()->loadTilesetImage(sheetName);
  } else {
    int count = 0;
    for (auto c: sheetName) {
      if (count >= 2) {
        break;
      }

      if (c == '$') {
        m_isSingleCharacter = true;
        ++count;
      } else if (c == '!') {
        m_noShift6_noHalfTransInBush = true;
        ++count;
      }
    }
    m_sheetTexture = ResourceManager::instance()->loadCharacterImage(sheetName);
  }
}

int CharacterSheet::characterWidth() const {
  return m_isTileId ? m_sheetTexture.width() / patternCountForTile() : m_sheetTexture.width() / patternCount();
}

int CharacterSheet::characterHeight() const {
  return m_isTileId ? m_sheetTexture.width() / directionCountForTile() : m_sheetTexture.height() / directionCount();
}

SimpleRect CharacterSheet::getRectForCharacter(int character, int pattern, Direction direction) const {
  if (!m_sheetTexture) {
    return {};
  }
  const int CharacterSpriteWidth = characterWidth();
  const int CharacterSpriteHeight = characterHeight();
  const int CharacterAtlasWidth = m_isSingleCharacter ? m_sheetTexture.width() : m_sheetTexture.width() / 4;
  const int CharacterAtlasHeight = m_isSingleCharacter ? m_sheetTexture.height() : m_sheetTexture.height() / 2;
  const float charX = static_cast<float>((character % (m_sheetTexture.width() / CharacterAtlasWidth)) *
                                         CharacterAtlasWidth);
  const float charY = static_cast<float>((character / (m_sheetTexture.width() / CharacterAtlasWidth)) *
                                         CharacterAtlasHeight);
  const float patternOffset = static_cast<float>(pattern * CharacterSpriteWidth);
  const float directionOffset = direction != Direction::Retain
                                  ? (((static_cast<int>(direction) - 2) / 2) * CharacterSpriteHeight)
                                  : 0.f;

  return {
    PointF{(charX + patternOffset) / m_sheetTexture.width(), (charY + directionOffset) / m_sheetTexture.height()},
    PointF{
      (charX + patternOffset + CharacterSpriteWidth) / m_sheetTexture.width(),
      (charY + directionOffset + CharacterSpriteHeight) / m_sheetTexture.height()
    },
  };
}

SimpleRect CharacterSheet::getRectForTile(int tileId) {
  if (!m_sheetTexture) {
    return {};
  }

  const int CharacterSpriteWidth = 48;
  const int CharacterSpriteHeight = 48;

  // Use 256-tile pages, assuming 16x16 tiles per page
  int localId = tileId % 256;
  int tileX = localId % 16;
  int tileY = localId / 16;

  // Compute pixel coordinates
  float tileU0 = tileX * CharacterSpriteWidth;
  float tileV0 = tileY * CharacterSpriteHeight;
  float tileU1 = tileU0 + static_cast<float>(CharacterSpriteWidth);
  float tileV1 = tileV0 + static_cast<float>(CharacterSpriteHeight);

  // Normalize UVs
  tileU0 /= m_sheetTexture.width();
  tileV0 /= m_sheetTexture.height();
  tileU1 /= m_sheetTexture.width();
  tileV1 /= m_sheetTexture.height();

  return {PointF{tileU0, tileV0}, PointF{tileU1, tileV1}};
}
