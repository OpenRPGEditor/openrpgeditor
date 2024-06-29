#pragma once

#include "Core/Texture.hpp"
#include "Database/Globals.hpp"

struct CharacterUV {
  float u;
  float v;
};
struct CharacterRect {
  CharacterUV uv0;
  CharacterUV uv1;
};

struct CharacterSheet {
  CharacterSheet() = delete;

  explicit CharacterSheet(std::string_view sheetName);
  int characterWidth() const;
  int characterHeight() const;

  [[nodiscard]] bool isSingleCharacter() const { return m_isSingleCharacter; }
  [[nodiscard]] bool isNoShift() const { return m_noShift6_noHalfTransInBush; }
  [[nodiscard]] bool isNoHalfTransparentInBush() const { return m_noShift6_noHalfTransInBush; }
  [[nodiscard]] CharacterRect getRectForCharacter(int character, int pattern = 0, Direction direction = Direction::Down) const;

  explicit operator bool() const { return m_sheetTexture.operator bool(); }

  [[nodiscard]] constexpr int patternCount() const { return m_isSingleCharacter ? 3 : 12; }
  [[nodiscard]] constexpr int directionCount() const { return m_isSingleCharacter ? 4 : 8; }

  const Texture& texture() const { return m_sheetTexture; }

private:
  bool m_isSingleCharacter = false;
  bool m_noShift6_noHalfTransInBush = false;
  Texture m_sheetTexture;
};
