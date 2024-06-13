#pragma once

#include "DatabaseEditor.hpp"
#include "EventListView.hpp"
#include "MapEditor.hpp"
#include "MapListView.hpp"
#include "TilesetPicker.hpp"
#include "UndoStack.hpp"

#include <string_view>
#include <string>
#include "Core/ResourceManager.hpp"
#include "Core/EventEditor.hpp"

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
  void handleOpenFile();
  void handleUndo();
  void handleRedo();

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

  std::string variable(int id) { return m_system.variable(id); }
  const std::string variable(int id) const { return m_system.variable(id); }

  std::string switche(int id) { return m_system.switche(id); }
  const std::string switche(int id) const { return m_system.switche(id); }
  CommonEvent* commonEvent(int id) { return m_commonEvents.event(id); }
  const CommonEvent* commonEvent(int id) const { return m_commonEvents.event(id); }

  Actor* actor(int id) { return m_actors.actor(id); }
  Event* event(int id) { return m_map->event(id); }
  Class* actorClass(int id) { return m_classes.classType(id); }
  const char* vehicle(int id) { return id == 0 ? "Boat" : id == 1 ? "Ship" : "Airship"; }
  Enemy* enemy(int id) { return m_enemies.enemy(id); }
  Skill* skill(int id) { return m_skills.skill(id); }
  Weapon* weapon(int id) { return m_weapons.weapon(id); }
  Armor* armor(int id) { return m_armors.armor(id); }
  Item* item(int id) { return m_items.item(id); }
  State* state(int id) { return m_states.state(id); }
  Animation* animation(int id) { return m_animations.animation(id); }

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

  void addUndo(const std::weak_ptr<IUndoCommand>& cmd) {
    m_undoStack.push(cmd);
    /* Clear the redo stack since adding a new undo command invalidates the redo state */
    m_redoStack.clear();
  }

  System& system() { return m_system; }
  const System& system() const { return m_system; }

  MapEditor* mapEditor() { return &m_mapEditor; }
  const MapEditor* mapEditor() const { return &m_mapEditor; }

private:
  void drawMenu();
  void drawFileDialog();
  void handleKeyboardShortcuts();

  MapListView m_mapListView;
  MapEditor m_mapEditor;
  EventListView m_eventListView;
  TilesetPicker m_tilesetPicker;
  UndoStack m_undoStack;
  UndoStack m_redoStack;
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

  EditMode m_editMode = EditMode::Event;
  DrawTool m_drawTool = DrawTool::Pencil;

  std::optional<ResourceManager> m_resourceManager;
  std::optional<Map> m_map;

  bool m_showDemoWindow{};
  bool m_showAboutWindow{};
  bool m_showTileDebug{true};
};