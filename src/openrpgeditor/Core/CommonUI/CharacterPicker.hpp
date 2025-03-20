#pragma once

#include "Core/CommonUI/IDialogController.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckerboardTexture.hpp"
#include "Database/Globals.hpp"
#include "Directory.hpp"

struct CharacterPicker : IDialogController {
  enum class PickerMode {
    Character,
    PatternAndDirection,
  };

  explicit CharacterPicker(const PickerMode mode = PickerMode::Character, std::string_view sheetName = {}, int character = 0, int pattern = 0, Direction direction = Direction::Down);
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] PickerMode pickerMode() const { return m_pickerMode; }
  [[nodiscard]] int selectedPattern() const { return m_pattern; }
  [[nodiscard]] Direction selectedDirection() const { return m_direction; }
  [[nodiscard]] int character() const { return m_characterIndex; }
  [[nodiscard]] std::string selectedSheet() const {
    return m_selectedSheet >= 0 ? m_charDir.value().isParentDirectory() ? m_characterSheets[m_selectedSheet] : m_charDir.value().getPathPrefix() + '/' + m_characterSheets[m_selectedSheet] : "";
  }

  void setCharacterInfo(std::string_view sheetName, int character = 0, int pattern = 0, Direction direction = Direction::Down);

private:
  std::optional<Directory> m_charDir;
  std::vector<std::string> m_folders;
  int m_selectedFolder{-1};

  std::optional<CharacterSheet> m_characterSheet;
  std::vector<std::string> m_characterSheets;
  PickerMode m_pickerMode;
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
