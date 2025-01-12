#pragma once
#include "Core/CommonUI/ImagePicker.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/TileMarker.hpp"
#include "Database/Tilesets.hpp"

class DBTilesetsTab final : public IDBEditorTab {
public:
  DBTilesetsTab() = delete;
  explicit DBTilesetsTab(Tilesets& tilesets, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Tileset>& tilesets() { return m_tilesets.tilesets(); }
  [[nodiscard]] const std::vector<Tileset>& tilesets() const { return m_tilesets.tilesets(); }

  [[nodiscard]] Tileset* tileset(const int id) { return m_tilesets.tileset(id); }
  [[nodiscard]] const Tileset* tileset(const int id) const { return m_tilesets.tileset(id); }

private:
  Tilesets& m_tilesets;
  Tileset* m_selectedTileset{};
  int m_maxTilesets{};
  int m_editMaxTilesets{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  int m_selectedTileTab{0};
  int m_pickerSelection;
  int m_checkerBoardHeight{0};
  int m_flagSelection{0};
  bool m_updateCheckerboard{true};

  std::optional<ImagePicker> m_imagePicker;
  std::optional<Image> m_image;
  std::optional<TileMarker> m_tileMarker;

  CheckerboardTexture m_checkerboardTexture{};
  CheckerboardTexture m_checkerboardTexture2{};

  const int m_tileSize = 48;                                     // Tile size in pixels
  const int m_gridCols = 16;                                     // Number of tiles per row
  const float m_scale = 1.f;                                     // Scaling factor
  static constexpr ImVec4 kDefaultTint{1.0f, 1.0f, 1.0f, 0.70f}; // 75% transparency
  static constexpr ImVec4 kHoveredTint{1.0f, 1.0f, 1.0f, 1.0f};  // Fully opaque

  void drawA1();
  void drawA2();
  void drawA3();
  void drawA4();
  void drawA5();
  void drawTileset(int type);
  void drawTileMarker(int flagType, ImVec2 tilePos, int tileIndex);
  void toggleTileState(int tileIndex, bool reverse, TileFlags subTileFlag = TileFlags::None);
  void toggleSelection(int flagIndex, int tileIndex, bool reverse);
  static int getTileId(const std::vector<std::array<int, 4>>& tileData);
};