#include "Editor/Graphics/CharacterSheet.hpp"

#include "Editor/ResourceManager.hpp"

#include "Database/Database.hpp"

CharacterSheet::CharacterSheet(std::string_view sheetName, const int tileId)
: m_characterName(sheetName)
, m_isTileId(tileId > 0) {
  if (tileId > 0) {
    const int setId = 5 + static_cast<int>(floorf(static_cast<float>(tileId) / 256));
    const Map* map = nullptr;
    if (Database::instance()->mapInfos->currentMap() && Database::instance()->mapInfos->currentMap()->map()) {
      map = Database::instance()->mapInfos->currentMap()->map();
    }
    const Tileset* tileset = nullptr;
    if (map) {
      tileset = Database::instance()->tilesets->tileset(map->tilesetId());
    }
    if (tileset) {
      sheetName = tileset->tilesetName(setId);
    }
    m_sheetTexture = ResourceManager::instance()->loadTilesetImage(sheetName);
  } else if (!sheetName.empty()) {
    int count = 0;
    for (auto c : sheetName) {
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

int CharacterSheet::characterWidth() const { return m_isTileId ? m_sheetTexture.width() / patternCountForTile() : m_sheetTexture.width() / patternCount(); }

int CharacterSheet::characterHeight() const { return m_isTileId ? m_sheetTexture.width() / directionCountForTile() : m_sheetTexture.height() / directionCount(); }

SimpleRect CharacterSheet::getRectForCharacter(int character, int pattern, Direction direction) const {
  if (!m_sheetTexture) {
    return {};
  }
  const auto CharacterSpriteWidth = characterWidth();
  const auto CharacterSpriteHeight = characterHeight();
  const auto CharacterAtlasWidth = m_isSingleCharacter ? m_sheetTexture.width() : m_sheetTexture.width() / 4;
  const auto CharacterAtlasHeight = m_isSingleCharacter ? m_sheetTexture.height() : m_sheetTexture.height() / 2;
  const auto charX = static_cast<float>((character % (m_sheetTexture.width() / CharacterAtlasWidth)) * CharacterAtlasWidth);
  const auto charY = static_cast<float>((character / (m_sheetTexture.width() / CharacterAtlasWidth)) * CharacterAtlasHeight);
  const auto patternOffset = static_cast<float>(pattern * CharacterSpriteWidth);
  const auto directionOffset = direction != Direction::Retain ? (((static_cast<int>(direction) - 2) / 2) * CharacterSpriteHeight) : 0.f;

  return {
      PointF{(charX + patternOffset) / m_sheetTexture.width(), (charY + directionOffset) / m_sheetTexture.height()},
      PointF{(charX + patternOffset + CharacterSpriteWidth) / m_sheetTexture.width(), (charY + directionOffset + CharacterSpriteHeight) / m_sheetTexture.height()},
  };
}

SimpleRect CharacterSheet::getRectForTile(const int tileId) const {
  if (!m_sheetTexture) {
    return {};
  }

  const int CharacterSpriteWidth = 48;
  const int CharacterSpriteHeight = 48;

  // Use 256-tile pages, assuming 16x16 tiles per page
  int tileX = fmod(floorf(tileId / 128), 2) * 8 + tileId % 8;
  int tileY = fmodf(floorf(tileId % 256 / 8), 16);

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
