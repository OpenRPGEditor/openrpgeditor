#pragma once

#include "CommonUI/ColorFlashPicker.hpp"
#include "CommonUI/ColorTonePicker.hpp"
#include "CommonUI/CreateNewProjectDialog.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/EventListView.hpp"
#include "Core/MapEditor.hpp"
#include "Core/MapListView.hpp"
#include "Core/NWJSVersionManager.hpp"
#include "Core/UndoStack.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/EventEditor.hpp"

#include "Core/CommonUI/TilesetPicker.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/TextEditor.hpp"

#include "Database/Database.hpp"
#include "Database/MapInfos.hpp"
#include "Graphics/ColorTonePreview.hpp"

#include <string_view>
#include <string>

class CreateNewProjectDialog;
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
  Project();

  bool load(std::string_view filePath, std::string_view basePath);
  void save();
  bool close(bool promptSave = false);
  void setupDocking();

  [[nodiscard]] bool isLoaded() const { return m_isLoaded; }
  [[nodiscard]] bool isValid() const { return m_isValid; }
  [[nodiscard]] const std::string& version() const { return m_database->projectVersion; }

  // Files will inform Project of that they've been modified via a callback,
  // this keeps things simple and avoids delegate overhead
  [[nodiscard]] bool modified() { return m_isModified; }

  void draw();
  void drawTileInfo(MapRenderer::MapLayer& mapLayer, int z);
  void handleOpenFile();
  void handleCreateNewProject();
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

  std::string variable(int id) { return m_database->system.variable(id); }
  const std::string variable(int id) const { return m_database->system.variable(id); }

  std::string switche(int id) { return m_database->system.switche(id); }
  const std::string switche(int id) const { return m_database->system.switche(id); }
  CommonEvent* commonEvent(int id) { return m_database->commonEvents.event(id); }
  const CommonEvent* commonEvent(int id) const { return m_database->commonEvents.event(id); }

  Actor* actor(int id) { return m_database->actors.actor(id); }
  Event* event(int id) { return m_mapEditor.map()->event(id); }
  Class* actorClass(int id) { return m_database->classes.classType(id); }
  const char* vehicle(int id) { return id == 0 ? "Boat" : id == 1 ? "Ship" : "Airship"; }
  Enemy* enemy(int id) { return m_database->enemies.enemy(id); }
  Skill* skill(int id) { return m_database->skills.skill(id); }
  Weapon* weapon(int id) { return m_database->weapons.weapon(id); }
  Armor* armor(int id) { return m_database->armors.armor(id); }
  Item* item(int id) { return m_database->items.item(id); }
  State* state(int id) { return m_database->states.state(id); }
  MapInfo* map(int id) { return m_database->mapInfos.map(id); }
  std::string equipType(int id) { return m_database->system.equipTypes.at(id); }
  Animation* animation(int id) { return m_database->animations.animation(id); }
  Tileset* tileset(int id) { return m_database->tilesets.tileset(id); }

  void setDrawTool(DrawTool tool) { m_drawTool = tool; }
  DrawTool drawTool() const { return m_drawTool; }

  MapInfo* currentMapInfo() { return m_mapListView.currentMapInfo(); }
  const MapInfo* currentMapInfo() const { return m_mapListView.currentMapInfo(); }

  Troop* troop(int id) { return m_database->troops.troop(id); }
  const Tileset* tileset(int id) const { return m_database->tilesets.tileset(id); }

  void setMap(MapInfo& in);

  Map* currentMap() { return m_mapEditor.map(); }

  const Map* currentMap() const { return m_mapEditor.map(); }

  void addUndo(const std::weak_ptr<IUndoCommand>& cmd) {
    m_undoStack.push(cmd);
    /* Clear the redo stack since adding a new undo command invalidates the redo state */
    m_redoStack.clear();
  }

  System& system() { return m_database->system; }
  const System& system() const { return m_database->system; }

  MapEditor* mapEditor() { return &m_mapEditor; }
  const MapEditor* mapEditor() const { return &m_mapEditor; }

  Items& items() { return m_database->items; }
  const Items& items() const { return m_database->items; }

  Armors& armors() { return m_database->armors; }
  const Armors& armors() const { return m_database->armors; }

  Weapons& weapons() { return m_database->weapons; }
  const Weapons& weapons() const { return m_database->weapons; }

  Actors& actors() { return m_database->actors; }
  const Actors& actors() const { return m_database->actors; }

  Troops& troops() { return m_database->troops; }
  const Troops& troops() const { return m_database->troops; }

  std::vector<std::optional<Event>> events() { return currentMap()->events; }
  std::vector<std::optional<Event>> events() const { return currentMap()->events; }

  Database& database() { return m_database.value(); }
  const Database& database() const { return m_database.value(); }

private:
  void drawMenu();
  void drawToolbar();
  void drawCreateNewProjectPopup();
  void drawTileDebugger();
  void handleKeyboardShortcuts();
  MapListView m_mapListView;
  MapEditor m_mapEditor;
  EventListView m_eventListView;
  TilesetPicker m_tilesetPicker;
  NWJSVersionManager m_nwjsVersionManager;
  UndoStack m_undoStack;
  UndoStack m_redoStack;
  bool m_isValid = false;
  bool m_isLoaded = false;
  bool m_isModified = false;
  CreateNewProjectDialog m_createNewProject = CreateNewProjectDialog("Create New Project");

  std::optional<Database> m_database;

  std::optional<DatabaseEditor> m_databaseEditor;

  EditMode m_editMode = EditMode::Event;
  DrawTool m_drawTool = DrawTool::Pencil;

  std::optional<ResourceManager> m_resourceManager;

  bool m_showDemoWindow{};
  bool m_showAboutWindow{};
  bool m_showTileDebug{false};

  float m_menuBarHeight{};
  float m_toolbarSize = 32;
  ColorTonePicker m_tonePicker{};
  ColorFlashPicker m_flashPicker{};
};