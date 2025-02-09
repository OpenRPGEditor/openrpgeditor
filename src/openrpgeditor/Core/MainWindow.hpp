#pragma once

#include "CommonUI/CreateNewProjectDialog.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/EventEditor.hpp"
#include "Core/EventListView.hpp"
#include "Core/MapEditor.hpp"
#include "Core/MapListView.hpp"
#include "Core/NWJSVersionManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UndoStack.hpp"
#include "EventSearcher.hpp"
#include "SettingsDialog.hpp"
#include "angelscript.h"

#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/CommonUI/TilesetPicker.hpp"

#include "Database/Database.hpp"
#include "Database/MapInfos.hpp"

#include <string>
#include <string_view>

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
  Eraser,
};

enum class ToolbarCategory {
  File,
  UI,
  MapOperation,
};

class asIScriptFunction;
class MainWindow {
public:
  class ToolbarButton {
  public:
    ToolbarButton(const std::string& id, const ToolbarCategory category, asIScriptFunction* func);
    ~ToolbarButton();
    const std::string& id() const { return m_id; }

    void callOnClicked() const;

  private:
    std::string m_id;
    ToolbarCategory m_category;
    asIScriptFunction* m_func;
    void* m_callbackObject = nullptr;
    asITypeInfo* m_callbackObjectType = nullptr;
  };
  MainWindow();
  ~MainWindow() { m_instance = nullptr; }

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

  MapEditor* mapEditor() { return &m_mapEditor; }
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

  std::vector<std::optional<Event>> events() { return currentMap()->events(); }
  std::vector<std::optional<Event>> events() const { return currentMap()->events(); }

  Database& database() { return m_database.value(); }
  const Database& database() const { return m_database.value(); }

  const TilesetPicker& tilesetPicker() const { return m_tilesetPicker; }

  void openMapProperties(MapInfo* mapInfo) {
    m_mapProperties.setMapInfo(mapInfo);
    m_mapProperties.setOpen(true);
  }

  void addToolbarButton(const std::string& id, ToolbarCategory category, asIScriptFunction* func);
  static void RegisterBindings();

  static MainWindow* instance() { return m_instance; }

private:
  void onActorsLoaded();
  void onClassesLoaded();
  void onSkillsLoaded();
  void onItemsLoaded();
  void onWeaponsLoaded();
  void onArmorsLoaded();
  void onEnemiesLoaded();
  void onTroopsLoaded();
  void onStatesLoaded();
  void onAnimationsLoaded();
  void onTilesetsLoaded();
  void onCommonEventsLoaded();
  void onSystemLoaded();
  void onGameConstantsLoaded();
  void onTemplatesLoaded();
  void onDocsLoaded();
  void onDatabaseReady();

  void drawMenu();
  void drawToolbar();
  void drawCreateNewProjectPopup();
  void drawTileDebugger();
  void handleKeyboardShortcuts();
  SettingsDialog m_settingsDialog;
  MapListView m_mapListView;
  MapProperties m_mapProperties;
  MapEditor m_mapEditor;
  EventListView m_eventListView;
  TilesetPicker m_tilesetPicker;
  NWJSVersionManager m_nwjsVersionManager;
  EventSearcher m_eventSearcher;
  UndoStack m_undoStack;
  UndoStack m_redoStack;
  bool m_isValid = false;
  bool m_isLoaded = false;
  bool m_isModified = false;

  std::unordered_map<ToolbarCategory, std::vector<ToolbarButton>> m_toolbarButtons;
  CreateNewProjectDialog m_createNewProject = CreateNewProjectDialog("Create New Project");
  std::optional<Database> m_database;

  std::optional<DatabaseEditor> m_databaseEditor;

  EditMode m_editMode = EditMode::Event;
  DrawTool m_drawTool = DrawTool::Pencil;

  bool m_showDemoWindow{};
  bool m_showAboutWindow{};
  bool m_showTileDebug{false};

  float m_menuBarHeight{};
  float m_toolbarButtonSize{};
  float m_toolbarSize = 32;
  static MainWindow* m_instance;
};