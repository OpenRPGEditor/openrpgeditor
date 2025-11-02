#pragma once
#include "Database/Tilesets.hpp"
#include "DBCommonEventsTab.hpp"
#include "Editor/CommonUI/ImagePicker.hpp"
#include "Editor/DatabaseEditor/IDBEditorTab.hpp"
#include "Editor/Graphics/TileMarker.hpp"

class DBTilesetsTab final : public IDBCoreEditorTab<DBTilesetsTab> {
public:
  void draw() override;

  std::string getName(const int index) const override { return m_tilesets->tileset(index)->name(); }
  int getCount() const override { return m_tilesets->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Tilesets"); }

  bool isExperimental() const override { return true; }

  bool isReady() const override { return !!Database::instance()->tilesets; }
  bool isInitialized() const override { return m_tilesets; }

  void initialize() override {
    if (!isReady()) {
      return;
    }

    m_tilesets = &Database::instance()->tilesets.value();
    m_selectedTileset = m_tilesets->tileset(1);
    m_tileMarker.emplace(TileFlags::None, 1, 256, 320);
    m_maxTilesets = m_tilesets->count();
  }

private:
  Tilesets* m_tilesets{};
  Tileset* m_selectedTileset{};
  int m_maxTilesets{};
  int m_editMaxTilesets{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  int m_selectedTileTab{0};
  int m_pickerSelection{};
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