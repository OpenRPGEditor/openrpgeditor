#pragma once

#include "DatabaseEditor.hpp"
#include "EventListView.hpp"
#include "MapEditor.hpp"
#include "MapListView.hpp"
#include "TilesetPicker.hpp"

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
  Project() : m_mapListView(this), m_mapEditor(this), m_eventListView(this), m_tilesetPicker(this) {}

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

  MapInfo* currentMapInfo() { return m_mapListView.currentMapInfo(); }
  const MapInfo* currentMapInfo() const { return m_mapListView.currentMapInfo(); }

  Tileset* tileset(int id) { return m_tilesets.tileset(id); }
  const Tileset* tileset(int id) const { return m_tilesets.tileset(id); }

  void setMap(MapInfo& in);

  Map* currentMap() {
    if (m_map) {
      return &m_map.value();
    }
    return nullptr;
  }

  const Map* currentMap() const {
    if (m_map) {
      return &m_map.value();
    }
    return nullptr;
  }

private:
  void drawMenu();
  void drawFileDialog();

  MapListView m_mapListView;
  MapEditor m_mapEditor;
  EventListView m_eventListView;
  TilesetPicker m_tilesetPicker;
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

  std::optional<DatabaseEditor> m_databaseEditor;

  EditMode m_editMode = EditMode::Map;
  DrawTool m_drawTool = DrawTool::Pencil;

  std::optional<ResourceManager> m_resourceManager;
  std::optional<Map> m_map;

  bool m_showDemoWindow{};
  bool m_showAboutWindow{};
  bool m_showTileDebug{true};
};