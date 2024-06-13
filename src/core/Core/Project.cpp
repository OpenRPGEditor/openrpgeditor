#include "Core/Project.hpp"

#include "ImGuiFileDialog.h"
#include "Settings.hpp"

#include "Core/Log.hpp"
#include "Core/Application.hpp"
#include "Core/Utils.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <fstream>

#include <array>
#include <string_view>
#include <algorithm>

using namespace std::literals::string_view_literals;
static SDL_Cursor* waitCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);

constexpr std::array KnownRPGMVVersions = {
    "RPGMV 1.0.0"sv,
    "RPGMV 1.6.1"sv,
    "RPGMV 1.6.2"sv,
    "RPGMV 1.6.3"sv,
};

bool Project::load(std::string_view filePath, std::string_view basePath) {
  m_undoStack.clear();
  m_redoStack.clear();
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
  m_projectVersion = version;
  m_basePath = basePath;
  m_projectFilePath = filePath;
  APP_INFO("Loading Actor definitions...");
  m_actors = Actors::load(m_basePath + "/data/Actors.json");
  APP_INFO("Loading Class definitions...");
  m_classes = Classes::load(m_basePath + "/data/Classes.json");
  APP_INFO("Loading Skill definitions...");
  m_skills = Skills::load(m_basePath + "/data/Skills.json");
  APP_INFO("Loading Item definitions...");
  m_items = Items::load(m_basePath + "/data/Items.json");
  APP_INFO("Loading Weapon definitions...");
  m_weapons = Weapons::load(m_basePath + "/data/Weapons.json");
  APP_INFO("Loading Armor definitions...");
  m_armors = Armors::load(m_basePath + "/data/Armors.json");
  APP_INFO("Loading Enemy definitions...");
  m_enemies = Enemies::load(m_basePath + "/data/Enemies.json");
  APP_INFO("Loading Troop definitions...");
  m_troops = Troops::load(m_basePath + "/data/Troops.json");
  APP_INFO("Loading State definitions...");
  m_states = States::load(m_basePath + "/data/States.json");
  APP_INFO("Loading Animation definitions...");
  m_animations = Animations::load(m_basePath + "/data/Animations.json");
  APP_INFO("Loading Tileset definitions...");
  m_tilesets = Tilesets::load(m_basePath + "/data/Tilesets.json");
  APP_INFO("Loading CommonEvent definitions...");
  m_commonEvents = CommonEvents::load(m_basePath + "/data/CommonEvents.json");
  APP_INFO("Loading System...");
  m_system = System::load(m_basePath + "/data/System.json");
  m_mapInfos = MapInfos::load(m_basePath + "/data/MapInfos.json");
  m_databaseEditor.emplace(this, m_actors, m_classes, m_skills, m_items, m_weapons, m_armors, m_enemies, m_troops,
                           m_states, m_animations, m_tilesets, m_commonEvents, m_system);

  MapInfo* info = m_mapInfos.map(0);
  info->expanded = true;
  info->name = m_system.gameTitle;
  APP_INFO("Loaded project!");
  m_resourceManager.emplace(m_basePath);
  // Load the previously loaded map
  // m_dummyTex = m_resourceManager->loadParallaxImage("Map509");
  SDL_SetCursor(SDL_GetDefaultCursor());
  m_mapEditor.clearLayers();
  m_mapListView.setMapInfos(&m_mapInfos);

  MapInfo* m = m_mapInfos.map(m_system.editMapId);
  if (m != nullptr) {
    setMap(*m);
  }
  auto fileIt = std::find_if(Settings::instance()->mru.begin(), Settings::instance()->mru.end(),
                             [&filePath](const auto& t) { return t.first == filePath.data(); });
  if (fileIt == Settings::instance()->mru.end()) {
    Settings::instance()->mru.emplace_front(filePath.data(), m_system.gameTitle);
  } else {
    Settings::instance()->mru.erase(fileIt);
    Settings::instance()->mru.emplace_front(filePath.data(), m_system.gameTitle);
  }
  return true;
}

bool Project::close(bool save) {
  if (save) {
    // TODO: Implement when safe to do so
  }

  m_undoStack.clear();
  m_redoStack.clear();

  /* Default initialize all of these */
  m_databaseEditor.reset();
  m_actors = {};
  m_classes = {};
  m_skills = {};
  m_items = {};
  m_weapons = {};
  m_armors = {};
  m_enemies = {};
  m_troops = {};
  m_states = {};
  m_animations = {};
  m_tilesets = {};
  m_commonEvents = {};
  m_system = {};
  m_mapInfos = {};

  m_map.reset();
  m_resourceManager.reset();
  m_mapListView.reset();

  return true;
}

void Project::setupDocking() {
  ImGuiID mainWindowGroup = ImGui::GetID("MainWindowGroup");
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
                                                    static_cast<int>(ImGuiDockNodeFlags_NoTabBar));
    // 7. We're done setting up our docking configuration:
    ImGui::DockBuilderFinish(mainWindowGroup);
  }
  ImGui::DockSpaceOverViewport(ImGui::GetID("MainWindowGroup"));
}

void Project::draw() {
  if (m_databaseEditor) {
    m_databaseEditor->draw();
  }
  drawMenu();
  drawFileDialog();
  setupDocking();
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
  IGFD::FileDialogConfig config;
  config.path = Settings::instance()->lastDirectory.empty() ? "." : Settings::instance()->lastDirectory;
  ImGuiFileDialog::Instance()->OpenDialog("OpenProjectDlg", "Select a Project to Open", ".rpgproject", config);
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
      if (ImGui::MenuItem("New Project...", "Ctrl+N")) {
        // TODO: Implement project creation
      }
      if (ImGui::MenuItem("Open Project...", "Ctrl+O")) {
        handleOpenFile();
      }
      if (ImGui::MenuItem("Close Project...")) {
        close();
      }
      if (ImGui::MenuItem("Save Project...", "Ctlr+S")) {
        // TODO: Implement project saving
      }
      ImGui::Separator();
      if (ImGui::BeginMenu("Recent Projects", !Settings::instance()->mru.empty())) {
        if (ImGui::MenuItem("Clear")) {
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
      if (ImGui::MenuItem("Deployment...")) {
        // TODO: Implement game deployment
      }
      /*
      if (ImGui::MenuItem("Steam Management")) {
        // TODO: Debate on implementing this, it may not be wise, or worthwhile
      }
      */
      ImGui::Separator();
      if (ImGui::MenuItem("Exit", "Ctrl+Q")) {
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
      if (ImGui::MenuItem("Events Within Tile", "F7", m_mapEditor.prisonMode())) {
        m_mapEditor.togglePrisonMode();
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
      if (ImGui::MenuItem("Zoom In", "Ctrl++")) {
        m_mapEditor.scale(0.25f);
      }
      if (ImGui::MenuItem("Zoom Out", "Ctrl+-")) {
        m_mapEditor.scale(-0.25f);
      }
      if (ImGui::MenuItem("Actual Size", "Ctrl+0")) {
        m_mapEditor.setScale(1.f);
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Tools")) {
      if (ImGui::MenuItem("Database", "F9", false, m_databaseEditor != std::nullopt)) {
        m_databaseEditor->open();
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
        snprintf(buff, 4096, "open \"%s\"", m_basePath.c_str());
#elif _WIN32
        char buff[4096]{};
        snprintf(buff, 4096, "start \"%s\"", m_basePath.c_str());
#else
        char buff[4096]{};
        snprintf(buff, 4096, "xdg-open \"%s\"", m_basePath.c_str());
#endif
        if (strlen(buff) > 0) {
          ::system(buff);
        }
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

void Project::drawFileDialog() {
  // First check if we have a pending project request
  if (ImGuiFileDialog::Instance()->Display(
          "OpenProjectDlg", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal,
          ImVec2(600, 600))) {
    if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      Settings::instance()->lastDirectory = filePath;
      load(filePathName, filePath);
      // action
    }

    // close
    ImGuiFileDialog::Instance()->Close();
  }
}
void Project::setMap(MapInfo& in) {
  if (in.id == 0) {
    m_map.reset();
    m_mapListView.setCurrentMapId(0);
    m_mapEditor.setMap(nullptr, nullptr);
    return;
  }

  SDL_SetCursor(waitCursor);
  m_mapListView.setCurrentMapId(in.id);
  m_map = m_resourceManager->loadMap(in.id);
  if (m_map) {
    m_mapEditor.setMap(&m_map.value(), &in);
  } else {
    m_mapEditor.setMap(nullptr, nullptr);
  }

  SDL_SetCursor(SDL_GetDefaultCursor());
}