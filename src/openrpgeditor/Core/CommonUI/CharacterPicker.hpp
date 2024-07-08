#pragma once

#include "Core/CharacterSheet.hpp"
#include "Core/CommonUI/IDialogController.hpp"
#include "Core/Texture.hpp"
#include "Core/CheckerboardTexture.hpp"
#include "Database/Globals.hpp"

struct CharacterPicker : IDialogController {
  enum class PickerMode {
    Character,
    PatternAndDirection,
  };

  explicit CharacterPicker(const PickerMode mode = PickerMode::Character, std::string_view sheetName = {},
                           int character = 0, int pattern = 0, Direction direction = Direction::Down);
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] PickerMode pickerMode() const { return m_pickerMode; }
  [[nodiscard]] int selectedPattern() const { return m_pattern; }
  [[nodiscard]] Direction selectedDirection() const { return m_direction; }
  [[nodiscard]] int character() const { return m_characterIndex; }
  [[nodiscard]] std::string_view selectedSheet() const { return m_characterSheetName; }

private:
  std::optional<CharacterSheet> m_characterSheet;
  std::vector<std::string> m_characterSheets;
  PickerMode m_pickerMode;
  std::string m_characterSheetName;
  int m_characterIndex{0};
  CheckerboardTexture m_checkerboardTexture{};
  int m_selectedSheet{-1};
  int m_selectionWidth{48};
  int m_selectionHeight{48};
  int m_selectionX{0};
  int m_selectionY{0};
  // For PatternAndDirection mode
  int m_pattern{0};
  Direction m_direction{Direction::Down};
};
