#pragma once

#include "Core/CommonUI/IDialogController.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckerboardTexture.hpp"
#include "Database/Globals.hpp"
#include "Directory.hpp"
#include "Core/Tilemap/TilePalette.hpp"

struct CharacterPicker : IDialogController {
  enum class PickerMode {
    Character,
    PatternAndDirection,
  };

  explicit CharacterPicker(PickerMode mode = PickerMode::Character, bool useMapTiles = false, int tileId = 0,
                           std::string_view sheetName = {}, int character = 0, int pattern = 0,
                           Direction direction = Direction::Down);

  std::tuple<bool, bool> draw() override;

  [[nodiscard]] PickerMode pickerMode() const { return m_pickerMode; }
  [[nodiscard]] int selectedPattern() const { return m_pattern; }
  [[nodiscard]] Direction selectedDirection() const { return m_direction; }
  [[nodiscard]] int character() const { return m_characterIndex; }
  [[nodiscard]] bool isTilePick() const { return m_isTile; }
  [[nodiscard]] int tileId() const { return m_tileId; }

  [[nodiscard]] std::string selectedSheet() const {
    return m_selectedSheet >= 0
             ? m_charDir.value().isParentDirectory()
                 ? m_characterSheets[m_selectedSheet]
                 : m_charDir.value().getPathPrefix() + '/' + m_characterSheets[m_selectedSheet]
             : "";
  }

  [[nodiscard]] std::string selectedTileSheet() const {
    return m_tileSheets[m_selectedSheet + 9];
  }

  void setCharacterInfo(std::string_view sheetName, int character = 0, int pattern = 0,
                        Direction direction = Direction::Down);

  void setTileId(int tileId);

  Rect getTilesetRect();

private:
  std::optional<Directory> m_charDir;
  std::vector<std::string> m_folders;
  int m_selectedFolder{-1};

  bool m_isTile{false};
  int m_tileId{0};
  TilePalette m_palette;
  int m_tileX{0};
  int m_tileY{0};

  std::string m_filter;
  std::map<int, std::string> m_sortedIndexes;
  bool m_sortRequest{false};

  std::optional<CharacterSheet> m_characterSheet;
  std::vector<std::string> m_characterSheets;
  std::array<std::string, 9> m_tileSheets;
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

  int sheetIndexOf(std::string &str);
};
