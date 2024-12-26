#pragma once
#include "Core/CommonUI/ImagePicker.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
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
  int checkerBoardHeight{0};
  bool m_updateCheckerboard{true};

  std::optional<ImagePicker> m_imagePicker;
  std::optional<Image> m_image;
  CheckerboardTexture m_checkerboardTexture{};
  CheckerboardTexture m_checkerboardTexture2{};

  const int tileSize = 48;                             // Tile size in pixels
  const int gridCols = 16;                             // Number of tiles per row
  const float scale = App::DPIHandler::get_ui_scale(); // Scaling factor

  void drawA1();
  void drawA2();
  void drawA3();
  void drawA4();
  void drawA5();
  void drawTileset(int type);
};