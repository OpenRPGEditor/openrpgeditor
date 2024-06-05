#pragma once

#include "Runtime/DatabaseEditor.hpp"
#include <string_view>
#include <string>
#include "Core/ResourceManager.hpp"
#include "Core/MapRenderer.hpp"
#include "Core/Texture.hpp"

#include "Database/Actors.hpp"
#include "Database/Classes.hpp"
#include "Database/Skills.hpp"
#include "Database/Items.hpp"
#include "Database/Weapons.hpp"
#include "Database/Armors.hpp"
#include "Database/Enemies.hpp"
#include "Database/Troops.hpp"
#include "Database/States.hpp"
#include "Database/Animations.hpp"
#include "Database/Tilesets.hpp"
#include "Database/CommonEvents.hpp"
#include "Database/System.hpp"
#include "Database/MapInfos.hpp"

enum class EditMode {
  Map,
  Event,
};

enum class DrawTool {
  Pencil,
  Rectangle,
  Ellipse,
  Flood_Fill,
  Shadow_Pen,
};

class Project {
public:
  bool load(std::string_view filePath, std::string_view basePath);
  bool close(bool save = false);
  void setupDocking();

  [[nodiscard]] bool isLoaded() const { return m_isLoaded; }
  [[nodiscard]] bool isValid() const { return m_isValid; }
  [[nodiscard]] const std::string& version() const { return m_projectVersion; }

  // Files will inform Project of that they've been modified via a callback,
  // this keeps things simple and avoids delegate overhead
  [[nodiscard]] bool modified() { return m_isModified; }

  void draw();

  EditMode editMode() const { return m_editMode; }

  void enterMapEditMode() {
    if (m_editMode == EditMode::Map) {
      return;
    }

    // TODO: Fully implement
    m_editMode = EditMode::Map;
  }

  void enterEventEditMode() {
    if (m_editMode == EditMode::Event) {
      return;
    }

    // TODO: Fully implement
    m_editMode = EditMode::Event;
  }

  void setDrawTool(DrawTool tool) { m_drawTool = tool; }
  DrawTool drawTool() const { return m_drawTool; }

private:
  void drawMenu();
  void drawFileDialog();
  void drawTilesets();
  void drawEventList();
  void doMapSelection(MapInfos::MapInfo& in);
  void drawMapTree();
  void drawMapEditor();
  void recursiveDrawTree(MapInfos::MapInfo& in);

  bool m_isValid = false;
  bool m_isLoaded = false;
  bool m_isModified = false;

  std::string m_projectVersion; // As stored in the .rpgproject file
  std::string m_projectFilePath;
  std::string m_basePath;

  Actors m_actors{};
  Classes m_classes{};
  Skills m_skills{};
  Items m_items{};
  Weapons m_weapons{};
  Armors m_armors{};
  Enemies m_enemies{};
  Troops m_troops{};
  States m_states{};
  Animations m_animations{};
  Tilesets m_tilesets{};
  CommonEvents m_commonEvents{};
  System m_system{};
  MapInfos m_mapInfos{};

  EditMode m_editMode = EditMode::Map;
  DrawTool m_drawTool = DrawTool::Pencil;

  std::optional<ResourceManager> m_resourceManager;
  std::optional<Map> m_map;
  int m_selectedMapId = -1;
  float m_mapScale = 1.f;
  MapRenderer m_mapRenderer;
};