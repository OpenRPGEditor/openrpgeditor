#include "Core/Graphics/CharacterSheet.hpp"

#include "Core/ResourceManager.hpp"

CharacterSheet::CharacterSheet(std::string_view sheetName, bool isTileId, int tileId) : m_characterName(sheetName) {
  if (sheetName.empty()) {
    if (tileId < 0) {
      return;
    }
  }

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

  m_isTileId = isTileId;
  m_tileId = tileId;

  if (m_tileId < 256) {
    // Tile B (0 - 255)
  } else if (m_tileId < 512) {
    // Tile C (256 - 511)
    m_tileId -= 256;
  } else if (m_tileId < 768) {
    // Tile D (512 - 767)
    m_tileId -= 512;
  } else if (m_tileId < 1024) {
    // Tile E (768 - 1023)
    m_tileId -= 768;
  }

  if (m_isTileId) {
    m_sheetTexture = ResourceManager::instance()->loadTilesetImage(sheetName);
  } else {
    m_sheetTexture = ResourceManager::instance()->loadCharacterImage(sheetName);
  }
}

int CharacterSheet::characterWidth() const {
  return m_isTileId ? m_sheetTexture.width() / patternCountForTile() : m_sheetTexture.width() / patternCount();
}

int CharacterSheet::characterHeight() const {
  return m_isTileId
           ? m_sheetTexture.width() / directionCountForTile()
           : m_sheetTexture.height() / directionCount();
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
  if (tileId < 256) {
  } else if (tileId < 512) {
    // Tile C (256 - 511)
    tileId -= 256;
  } else if (tileId < 768) {
    // Tile D (512 - 767)
    tileId -= 512;
  } else if (tileId < 1024) {
    // Tile E (768 - 1023)
    tileId -= 768;
  }
  const int CharacterSpriteWidth = 48;
  const int CharacterSpriteHeight = 48;
  const int GridSize = 16;
  const int AtlasWidth = GridSize * CharacterSpriteWidth;
  const int AtlasHeight = GridSize * CharacterSpriteHeight;

  // Extract column and row from tileId
  const int tileColumn = tileId % GridSize;
  const int tileRow = tileId / GridSize;

  // Compute pixel position
  const int tileX = tileColumn * CharacterSpriteWidth;
  const int tileY = tileRow * CharacterSpriteHeight;

  return {
    PointF{static_cast<float>(tileX) / AtlasWidth, static_cast<float>(tileY) / AtlasHeight},
    PointF{
      static_cast<float>(tileX + CharacterSpriteWidth) / AtlasWidth,
      static_cast<float>(tileY + CharacterSpriteHeight) / AtlasHeight
    }
  };
}
