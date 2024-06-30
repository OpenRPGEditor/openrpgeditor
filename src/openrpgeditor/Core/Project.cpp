#include "Core/Project.hpp"

#include "Settings.hpp"

#include "Core/Log.hpp"
#include "Core/Application.hpp"
#include "Core/Utils.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "IconsFontAwesome6.h"

#if _WIN32
#include <shellapi.h>
#endif

#include "ImGuiUtils.hpp"
#include "nfd.h"

#include <fstream>

#include <array>
#include <string_view>
#include <algorithm>

using namespace std::literals::string_view_literals;
static SDL_Cursor* waitCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);
// clang-format off
constexpr std::array KnownRPGMVVersions = {
    "RPGMV 1.0.0"sv,
    "RPGMV 1.6.1"sv,
    "RPGMV 1.6.2"sv,
    "RPGMV 1.6.3"sv,
    "RPGMZ 1.8.0"sv,
};
// clang-format on

bool Project::load(std::string_view filePath, std::string_view basePath) {
  close();
  SDL_SetCursor(waitCursor);
  std::string version;
  std::ifstream file(filePath.data());
  std::getline(file, version);
  Utils::trim(version);

  auto it = std::find(KnownRPGMVVersions.begin(), KnownRPGMVVersions.end(), version);

  m_isValid = it != KnownRPGMVVersions.end();
  if (!m_isValid) {
    APP_ERROR("Invalid project version {}", version);
    SDL_SetCursor(SDL_GetDefaultCursor());
    return false;
  }
  APP_INFO("Got project for {}", version);
  m_database.emplace();

  m_database->projectVersion = version;
  m_database->basePath = basePath;
  m_database->projectFilePath = filePath;
  APP_INFO("Loading Actor definitions...");
  m_database->actors = Actors::load(m_database->basePath + "/data/Actors.json");
  APP_INFO("Loading Class definitions...");
  m_database->classes = Classes::load(m_database->basePath + "/data/Classes.json");
  APP_INFO("Loading Skill definitions...");
  m_database->skills = Skills::load(m_database->basePath + "/data/Skills.json");
  APP_INFO("Loading Item definitions...");
  m_database->items = Items::load(m_database->basePath + "/data/Items.json");
  APP_INFO("Loading Weapon definitions...");
  m_database->weapons = Weapons::load(m_database->basePath + "/data/Weapons.json");
  APP_INFO("Loading Armor definitions...");
  m_database->armors = Armors::load(m_database->basePath + "/data/Armors.json");
  APP_INFO("Loading Enemy definitions...");
  m_database->enemies = Enemies::load(m_database->basePath + "/data/Enemies.json");
  APP_INFO("Loading Troop definitions...");
  m_database->troops = Troops::load(m_database->basePath + "/data/Troops.json");
  APP_INFO("Loading State definitions...");
  m_database->states = States::load(m_database->basePath + "/data/States.json");
  APP_INFO("Loading Animation definitions...");
  m_database->animations = Animations::load(m_database->basePath + "/data/Animations.json");
  APP_INFO("Loading Tileset definitions...");
  m_database->tilesets = Tilesets::load(m_database->basePath + "/data/Tilesets.json");
  APP_INFO("Loading CommonEvent definitions...");
  m_database->commonEvents = CommonEvents::load(m_database->basePath + "/data/CommonEvents.json");
  APP_INFO("Loading System...");
  m_database->system = System::load(m_database->basePath + "/data/System.json");
  APP_INFO("Loading Plugins...");
  m_database->plugins = Plugins::load(m_database->basePath + "js/plugins.js");
  APP_INFO("Loading GameConstants");
  m_database->gameConstants = GameConstants::load(m_database->basePath + "/data/Constants.json");
  m_database->gameConstants.serialize(m_database->basePath + "/data/Constants.json");
  APP_INFO("Loading MapInfos...");
  m_database->mapInfos = MapInfos::load(m_database->basePath + "/data/MapInfos.json");
  m_databaseEditor.emplace(this, m_database->actors, m_database->classes, m_database->skills, m_database->items,
                           m_database->weapons, m_database->armors, m_database->enemies, m_database->troops,
                           m_database->states, m_database->animations, m_database->tilesets, m_database->commonEvents,
                           m_database->system, m_database->gameConstants);

  MapInfo* info = m_database->mapInfos.map(0);
  info->expanded = true;
  info->name = m_database->system.gameTitle;
  APP_INFO("Loaded project!");
  m_resourceManager.emplace(m_database->basePath);
  // Load the previously loaded map
  SDL_SetCursor(SDL_GetDefaultCursor());
  m_mapEditor.clearLayers();
  m_mapListView.setMapInfos(&m_database->mapInfos);

  MapInfo* m = m_database->mapInfos.map(m_database->system.editMapId);
  if (m != nullptr) {
    setMap(*m);
  }

  auto fileIt = std::find_if(Settings::instance()->mru.begin(), Settings::instance()->mru.end(),
                             [&filePath](const auto& t) { return t.first == filePath.data(); });
  if (fileIt == Settings::instance()->mru.end()) {
    Settings::instance()->mru.emplace_front(filePath.data(), m_database->system.gameTitle);
  } else {
    Settings::instance()->mru.erase(fileIt);
    Settings::instance()->mru.emplace_front(filePath.data(), m_database->system.gameTitle);
  }
  m_isLoaded = true;
  return true;
}

bool Project::close(bool promptSave) {
  if (promptSave) {
    // TODO: Implement when safe to do so
    if (m_database) {
      m_database->gameConstants.serialize(m_database->basePath + "/data/Constants.json");
      if (m_database->gameConstants.generateJS) {
        m_database->gameConstants.generateConstantsJS(m_database->basePath + "/js/Constants.js");
      }
    }
  }

  m_undoStack.clear();
  m_redoStack.clear();

  /* Default initialize all of these */
  m_databaseEditor.reset();
  m_database.reset();

  m_mapEditor.setMap(nullptr);
  m_resourceManager.reset();
  m_mapListView.reset();
  m_isLoaded = false;

  return true;
}

void Project::setupDocking() {
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos + ImVec2(0, m_toolbarSize));
  ImGui::SetNextWindowSize(viewport->Size - ImVec2(0, m_toolbarSize));
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGuiWindowFlags window_flags = 0 | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
                                  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                  ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::Begin("Master DockSpace", nullptr, window_flags);
  ImGuiID mainWindowGroup = ImGui::GetID("MainWindowGroup");

  // Save off menu bar height for later.
  m_menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight;

  if (ImGui::DockBuilderGetNode(mainWindowGroup) == nullptr) {
    ImVec2 workPos = ImGui::GetMainViewport()->WorkPos;
    ImVec2 workSize = ImGui::GetMainViewport()->WorkSize;

    ImGui::DockBuilderRemoveNode(mainWindowGroup);
    ImGui::DockBuilderAddNode(mainWindowGroup);

    ImGui::DockBuilderSetNodeSize(mainWindowGroup, workSize);
    ImGui::DockBuilderSetNodePos(mainWindowGroup, workPos);
    ImGuiID dock1 = ImGui::DockBuilderSplitNode(mainWindowGroup, ImGuiDir_Left, 0.25f, nullptr, &mainWindowGroup);
    ImGuiID dock2 = ImGui::DockBuilderSplitNode(dock1, ImGuiDir_Down, 0.5f, nullptr, &dock1);
    ImGuiID dock3 = ImGui::DockBuilderSplitNode(mainWindowGroup, ImGuiDir_Right, 0.5f, nullptr, &mainWindowGroup);
    ImGuiID dock4 = ImGui::DockBuilderSplitNode(dock3, ImGuiDir_Right, 0.175f, nullptr, &dock3);
    // 6. Add windows to each docking space:
    ImGui::DockBuilderDockWindow("Events", dock1);
    ImGui::DockBuilderDockWindow("Tilesets", dock1);
    ImGui::DockBuilderDockWindow("Maps", dock2);
    ImGui::DockBuilderDockWindow("Map Editor", dock3);
    ImGui::DockBuilderDockWindow("Map Properties", dock4);
    ImGui::DockBuilderGetNode(dock3)->SetLocalFlags(static_cast<int>(ImGuiDockNodeFlags_NoUndocking) |
                                                    static_cast<int>(ImGuiDockNodeFlags_NoDocking));
    // 7. We're done setting up our docking configuration:
    ImGui::DockBuilderFinish(mainWindowGroup);
  }
  ImGui::DockSpace(ImGui::GetID("MainWindowGroup"));
  ImGui::End();
  ImGui::PopStyleVar(3);
}

void Project::drawToolbar() {
  const ImVec2 ButtonSize = ImVec2{32, 32} * App::DPIHandler::get_ui_scale();
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + m_menuBarHeight));
  ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, m_toolbarSize));
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGuiWindowFlags window_flags = 0 | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                  ImGuiWindowFlags_NoSavedSettings;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::Begin("##ore_toolbar", nullptr, window_flags);
  ImGui::PopStyleVar();

  ImGui::Button(ICON_FA_FILE, ButtonSize);
  if (ImGui::IsItemHovered()) {
    ImGui::ActionTooltip("New Project", "Creates a new project.");
  }
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_FOLDER_OPEN, ButtonSize)) {
    handleOpenFile();
  }
  if (ImGui::IsItemHovered()) {
    ImGui::ActionTooltip("Open Project", "Opens an existing project.");
  }
  ImGui::SameLine();
  ImGui::Button(ICON_FA_FLOPPY_DISK, ButtonSize);
  ImGui::SameLine();
  if (ImGui::IsItemHovered()) {
    ImGui::ActionTooltip("Save Project", "Saves the project.");
  }
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_SCISSORS, ButtonSize)) {}
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_COPY, ButtonSize)) {}
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_PASTE, ButtonSize)) {}
  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();
  ImGui::BeginDisabled(!m_undoStack.hasCommands());
  {
    if (ImGui::Button(ICON_FA_ARROW_ROTATE_LEFT, ButtonSize)) {
      handleUndo();
    }
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  ImGui::BeginDisabled(!m_redoStack.hasCommands());
  {
    if (ImGui::Button(ICON_FA_ARROW_ROTATE_RIGHT, ButtonSize)) {
      handleRedo();
    }
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();
  ImGui::BeginDisabled(m_editMode == EditMode::Map);
  {
    if (ImGui::Button("Map Mode")) {
      enterMapEditMode();
    }
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  ImGui::BeginDisabled(m_editMode == EditMode::Event);
  {
    if (ImGui::Button("Event Mode")) {
      enterEventEditMode();
    }
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();
  ImGui::BeginDisabled(editMode() != EditMode::Map || m_drawTool == DrawTool::Pencil);
  {
    if (ImGui::Button(ICON_FA_PENCIL, ButtonSize)) {
      setDrawTool(DrawTool::Pencil);
    }
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  ImGui::BeginDisabled(editMode() != EditMode::Map || m_drawTool == DrawTool::Rectangle);
  {
    if (ImGui::Button(ICON_FA_SQUARE, ButtonSize)) {
      setDrawTool(DrawTool::Rectangle);
    }
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  ImGui::BeginDisabled(editMode() != EditMode::Map || m_drawTool == DrawTool::Flood_Fill);
  {
    if (ImGui::Button(ICON_FA_BUCKET, ButtonSize)) {
      setDrawTool(DrawTool::Flood_Fill);
    }
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  ImGui::BeginDisabled(editMode() != EditMode::Map || m_drawTool == DrawTool::Shadow_Pen);
  {
    if (ImGui::Button(ICON_FA_PEN, ButtonSize)) {
      setDrawTool(DrawTool::Shadow_Pen);
    }
  }
  if (ImGui::IsItemHovered()) {
    ImGui::ActionTooltip("Shadow Pen", "Adds or removes shadows of walls");
  }
  ImGui::EndDisabled();
  ImGui::End();
}

void Project::draw() {
  m_toolbarSize = App::DPIHandler::scale_value(Settings::instance()->fontSize + 32);
  if (m_databaseEditor) {
    m_databaseEditor->draw();
  }
  drawMenu();
  setupDocking();
  drawToolbar();
  m_mapEditor.draw();

  if (m_editMode == EditMode::Map) {
    m_tilesetPicker.draw();
  } else {
    m_eventListView.draw();
  }

  m_mapListView.draw();

  if (m_showDemoWindow) {
    ImGui::ShowDemoWindow(&m_showDemoWindow);
  }
  if (m_showAboutWindow) {
    ImGui::ShowAboutWindow(&m_showAboutWindow);
  }
}

void Project::handleOpenFile() {
  nfdu8char_t* outPath;
  nfdu8filteritem_t filters = {"RPG Maker Projects", "rpgproject,rmmzproject"};
  auto result =
      NFD_OpenDialogU8(&outPath, &filters, 1,
                       Settings::instance()->lastDirectory.empty() ? "." : Settings::instance()->lastDirectory.c_str());
  if (result == NFD_OKAY) {
    std::filesystem::path path{outPath};
    Settings::instance()->lastDirectory = absolute(path).remove_filename().generic_string();
    load(absolute(path).generic_string(), Settings::instance()->lastDirectory);
    NFD_FreePathU8(outPath);
  }
}

void Project::handleUndo() {
  if (!m_undoStack.hasCommands()) {
    return;
  }

  auto cmd = m_undoStack.pop();
  if (cmd) {
    cmd->undo();
    cmd->setIsRedo(true);
    m_redoStack.push(cmd);
  }
}

void Project::handleRedo() {
  if (!m_redoStack.hasCommands()) {
    return;
  }

  auto cmd = m_redoStack.pop();
  if (cmd) {
    cmd->undo();
    cmd->setIsRedo(false);
    m_undoStack.push(cmd);
  }
}
void Project::drawMenu() {
  std::string loadFilepath;
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem(ICON_FA_FILE "  New Project...", "Ctrl+N")) {
        // TODO: Implement project creation
      }
      if (ImGui::MenuItem(ICON_FA_FOLDER " Open Project...", "Ctrl+O")) {
        handleOpenFile();
      }
      if (ImGui::MenuItem(ICON_FA_FOLDER_CLOSED " Close Project...")) {
        close();
      }
      if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save Project...", "Ctlr+S")) {
        // TODO: Implement project saving
      }
      ImGui::Separator();
      if (ImGui::BeginMenu("Recent Projects", !Settings::instance()->mru.empty())) {
        if (ImGui::MenuItem(ICON_FA_BRUSH " Clear")) {
          Settings::instance()->mru.clear();
        }
        if (!Settings::instance()->mru.empty()) {
          ImGui::Separator();
        }
        for (const auto& s : Settings::instance()->mru) {
          ImGui::PushID(s.first.c_str());
          if (ImGui::MenuItem(s.second.c_str())) {
            loadFilepath = s.first;
          }
          ImGui::PopID();
          if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("%s", s.first.c_str());
          }
        }
        ImGui::EndMenu();
      }
      ImGui::Separator();
      if (ImGui::MenuItem(ICON_FA_BOXES_PACKING " Deployment...")) {
        // TODO: Implement game deployment
      }
      /*
      if (ImGui::MenuItem("Steam Management")) {
        // TODO: Debate on implementing this, it may not be wise, or worthwhile
      }
      */
      ImGui::Separator();
      if (ImGui::MenuItem(ICON_FA_DOOR_CLOSED "  Exit", "Ctrl+Q")) {
        App::APP->stop();
      }
      ImGui::EndMenu();
    }

    /* TODO: Implement shortcut handling, ImGui doesn't handle that for various reasons
     * so we have to do that ourselves
     */
    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "Ctrl+Z", false, m_undoStack.hasCommands())) {
        handleUndo();
      }
      if (ImGui::IsItemHovered() && m_undoStack.hasCommands()) {
        ImGui::SetTooltip("%s", m_undoStack.top()->description().c_str());
      }

      if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z", false, m_redoStack.hasCommands())) {
        handleRedo();
      }
      if (ImGui::IsItemHovered() && m_redoStack.hasCommands()) {
        ImGui::SetTooltip("%s", m_redoStack.top()->description().c_str());
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "Ctrl+X")) {
        // TODO: Implement clipboard
      }
      if (ImGui::MenuItem("Copy", "Ctrl+C")) {
        // TODO: Implement clipboard
      }
      if (ImGui::MenuItem("Paste", "Ctrl+V")) {
        // TODO: Implement clipboard
      }
      if (ImGui::MenuItem("Delete", "Del")) {
        // TODO: Implement
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Find", "Ctrl+F")) {
        // TODO: Implement
      }
      if (ImGui::MenuItem("Find Next", "F3")) {
        // TODO: Implement
      }
      if (ImGui::MenuItem("Find Previous", "Shift+F3")) {
        // TODO: Implement
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Mode")) {
      if (ImGui::MenuItem("Map", "F5", editMode() == EditMode::Map)) {
        enterMapEditMode();
      }
      if (ImGui::MenuItem("Event", "F6", editMode() == EditMode::Event)) {
        enterEventEditMode();
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Game Preview", "F7", m_mapEditor.prisonMode())) {
        m_mapEditor.togglePrisonMode();
      }
      if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Toggles event borders, layer render order, and animations");
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Draw")) {
      for (auto v : magic_enum::enum_values<DrawTool>()) {
        if (ImGui::MenuItem(DecodeEnumName(magic_enum::enum_name(v)).data(), nullptr, drawTool() == v,
                            editMode() == EditMode::Map)) {
          setDrawTool(v);
        }
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Scale")) {
      if (ImGui::MenuItem("Zoom In", "Ctrl++", false, m_mapEditor.zoom() > 0.25f)) {
        m_mapEditor.scale(0.25f);
      }
      if (ImGui::MenuItem("Zoom Out", "Ctrl+-", false, m_mapEditor.zoom() < 4.f)) {
        m_mapEditor.scale(-0.25f);
      }
      if (ImGui::MenuItem("Actual Size", "Ctrl+0", false, m_mapEditor.zoom() != 1.f)) {
        m_mapEditor.setScale(1.f);
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Tools")) {
      if (ImGui::MenuItem("Database", "F9", false, m_databaseEditor != std::nullopt)) {
        m_databaseEditor->open();
      }

      /* Add tools above this */
      ImGui::Separator();
      if (ImGui::MenuItem("Reset Window Layout")) {
        ImGui::ClearIniSettings();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Game")) {
      if (ImGui::MenuItem("Play Test", "Ctrl+R", false, m_databaseEditor != std::nullopt)) {
        // TODO: Implement
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Open Folder", nullptr, false, m_databaseEditor != std::nullopt)) {
        /* is there a better way to do this? */
#if __APPLE__
        char buff[4096]{};
        snprintf(buff, 4096, "open \"%s\"", m_database->basePath.c_str());
        if (strlen(buff) > 0) {
          ::system(buff);
        }
#elif _WIN32
        ShellExecute(NULL, "open", m_database->basePath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
#else
        char buff[4096]{};
        snprintf(buff, 4096, "xdg-open \"%s\"", m_database->basePath.c_str());
        if (strlen(buff) > 0) {
          ::system(buff);
        }
#endif
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help")) {
      if (ImGui::MenuItem("Contents", "F1")) {}
      if (ImGui::MenuItem("Tutorials...")) {}
      ImGui::Separator();
      if (ImGui::MenuItem("About", nullptr, &m_showAboutWindow)) {}
      if (ImGui::MenuItem("Show Tile Debugger", nullptr, &m_showTileDebug)) {}
      if (ImGui::MenuItem("Show Demo", nullptr, &m_showDemoWindow)) {}
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  if (!loadFilepath.empty()) {
    load(loadFilepath, std::filesystem::absolute(loadFilepath).remove_filename().generic_string());
  }
  handleKeyboardShortcuts();
}

void Project::handleKeyboardShortcuts() {
  /* File Menu */
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      ImGui::IsKeyPressed(ImGuiKey_N)) {
    // TODO: implement new project creation
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      ImGui::IsKeyPressed(ImGuiKey_O)) {
    handleOpenFile();
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      ImGui::IsKeyPressed(ImGuiKey_S)) {
    // TODO: implement saving
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      ImGui::IsKeyReleased(ImGuiKey_O)) {
    handleOpenFile();
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      ImGui::IsKeyPressed(ImGuiKey_Q)) {
    App::APP->stop();
  }

  /* Edit Menu */
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      !(ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) &&
      ImGui::IsKeyPressed(ImGuiKey_Z)) {
    handleUndo();
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) &&
      ImGui::IsKeyPressed(ImGuiKey_Z)) {
    handleRedo();
  }
  /* TODO Clipboard */
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      ImGui::IsKeyPressed(ImGuiKey_X)) {
    // TODO: Implement clipboard cut
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      ImGui::IsKeyPressed(ImGuiKey_C)) {
    // TODO: Implement clipboard copy
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      ImGui::IsKeyPressed(ImGuiKey_V)) {
    // TODO: Implement clipboard paste
  }

  if (ImGui::IsKeyReleased(ImGuiKey_Delete)) {
    // TODO: Implement Delete
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      ImGui::IsKeyPressed(ImGuiKey_F)) {
    // TODO: Implement Find
  }
  if (ImGui::IsKeyPressed(ImGuiKey_F3)) {
    // TODO: Implement Find Next
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) &&
      ImGui::IsKeyPressed(ImGuiKey_F3)) {
    // TODO: Implement Find Previous
  }

  /* Mode */
  if (ImGui::IsKeyPressed(ImGuiKey_F5)) {
    enterMapEditMode();
  }

  if (ImGui::IsKeyPressed(ImGuiKey_F6)) {
    enterEventEditMode();
  }
  if (ImGui::IsKeyPressed(ImGuiKey_F7)) {
    m_mapEditor.togglePrisonMode();
  }

  /* Scale */
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      (ImGui::IsKeyPressed(ImGuiKey_Equal) || ImGui::IsKeyPressed(ImGuiKey_KeypadAdd))) {
    m_mapEditor.scale(0.25f);
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      (ImGui::IsKeyPressed(ImGuiKey_Minus) || ImGui::IsKeyPressed(ImGuiKey_KeypadSubtract))) {
    m_mapEditor.scale(-0.25f);
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) &&
      (ImGui::IsKeyPressed(ImGuiKey_Keypad0) || ImGui::IsKeyPressed(ImGuiKey_0))) {
    m_mapEditor.setScale(1.f);
  }

  /* Tools */
  if (ImGui::IsKeyReleased(ImGuiKey_F9)) {
    m_databaseEditor->open();
  }
  // TODO: Add missing tools
  /* Game */
  if ((ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) &&
      ImGui::IsKeyPressed(ImGuiKey_R)) {
    // TODO: Implement Play Test
  }

  /* Help */
  if (ImGui::IsKeyPressed(ImGuiKey_F1)) {
    // TODO: Help
  }
}

void Project::setMap(MapInfo& in) {
  if (in.id == 0) {
    m_mapListView.setCurrentMapId(0);
    m_mapEditor.setMap(nullptr);
    return;
  }

  SDL_SetCursor(waitCursor);
  m_mapListView.setCurrentMapId(in.id);
  if (m_mapListView.currentMapInfo()) {
    m_mapEditor.setMap(&in);
    in.map()->serialize("Test.json");
  } else {
    m_mapEditor.setMap(nullptr);
  }

  // if (m_map) {
  //   printf("%zu bytes, %i w %i h, %lu layers\n", m_map->data.size(), m_map->width, m_map->height,
  //          m_map->data.size() / (m_map->width * m_map->height));
  //   for (int z = 0; z < m_map->data.size() / (m_map->width * m_map->height); z++) {
  //     printf("-----------------Map Layer %.4i-----------------\n", z);
  //     for (int y = 0; y < m_map->height; y++) {
  //       for (int x = 0; x < m_map->width; x++) {
  //         int tileId = (z * m_map->height + y) * m_map->width + x;
  //         printf("%.04i ", m_map->data[tileId]);
  //       }
  //       printf("\n");
  //     }
  //   }
  // }

  SDL_SetCursor(SDL_GetDefaultCursor());
}