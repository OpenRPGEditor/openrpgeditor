#pragma once

#include "Core/Graphics/SimpleRect.hpp"
#include "Core/Graphics/Texture.hpp"
#include "Database/Globals.hpp"

struct CharacterSheet {
  CharacterSheet() = default;

  explicit CharacterSheet(std::string_view sheetName, bool isTileId = false, int tileId = 0);

  int characterWidth() const;

  int characterHeight() const;

  int characterAtlasWidth() const { return characterWidth() * (m_isTileId ? patternCountForTile() : patternCountForCharacter()); }

  int characterAtlasHeight() const { return characterHeight() * (m_isTileId ? directionCountForTile() : directionCountForCharacter()); }

  [[nodiscard]] bool isSingleCharacter() const { return m_isSingleCharacter; }
  [[nodiscard]] bool isNoShift() const { return m_noShift6_noHalfTransInBush; }
  [[nodiscard]] bool isNoHalfTransparentInBush() const { return m_noShift6_noHalfTransInBush; }

  [[nodiscard]] SimpleRect getRectForCharacter(int character, int pattern = 0, Direction direction = Direction::Down) const;

  [[nodiscard]] SimpleRect getRectForTile(int tileId);

  explicit operator bool() const { return m_sheetTexture.operator bool(); }

  [[nodiscard]] int patternCount() const { return m_isSingleCharacter ? 3 : 12; }
  [[nodiscard]] static constexpr int patternCountForCharacter() { return 3; }
  [[nodiscard]] int directionCount() const { return m_isSingleCharacter ? 4 : 8; }
  [[nodiscard]] static constexpr int directionCountForCharacter() { return 4; }
  [[nodiscard]] static constexpr int patternCountForTile() { return 16; }
  [[nodiscard]] static constexpr int directionCountForTile() { return 16; }

  [[nodiscard]] bool isTileSheet() { return m_isTileId; }

  const Texture& texture() const { return m_sheetTexture; }

  int numCharacters() const { return (m_sheetTexture.width() / characterWidth()) + (m_sheetTexture.height() / characterHeight()); }

  std::string_view characterName() const { return m_characterName; }

  operator ImTextureID() const { return m_sheetTexture.operator ImTextureID(); }

private:
  bool m_isTileId{false};

  bool m_isSingleCharacter = false;
  bool m_noShift6_noHalfTransInBush = false;
  std::string m_characterName;
  Texture m_sheetTexture;
};
