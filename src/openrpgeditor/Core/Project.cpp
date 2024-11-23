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

#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "nfd.h"
#include "Database/Serializable/DeserializationQueue.hpp"
#include "Database/Serializable/SerializationQueue.hpp"

#include "Database/Versions.hpp"
#include "Graphics/GameWindowBackground.hpp"

#include <clip.h>

#include <fstream>

#include <array>
#include <string_view>
#include <algorithm>

using namespace std::literals::string_view_literals;
static SDL_Cursor* waitCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);

Project::Project()
: m_mapListView(this)
, m_mapEditor(this)
, m_eventListView(this)
, m_tilesetPicker(this)
, m_nwjsVersionManager("https://dl.nwjs.io") {}

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
  SerializationQueue::instance().reset();
  DeserializationQueue::instance().reset();
  SerializationQueue::instance().setBasepath(basePath);
  DeserializationQueue::instance().setBasepath(basePath);
  m_database.emplace(basePath, filePath, version);
  m_database->load();
  m_resourceManager.emplace(m_database->basePath);
  m_databaseEditor.emplace(this, m_database->actors, m_database->classes, m_database->skills, m_database->items,
                           m_database->weapons, m_database->armors, m_database->enemies, m_database->troops,
                           m_database->states, m_database->animations, m_database->tilesets, m_database->commonEvents,
                           m_database->system, m_database->gameConstants, m_database->templates);

  MapInfo* info = m_database->mapInfos.map(0);
  info->expanded = true;
  info->name = m_database->system.gameTitle;
  APP_INFO("Loaded project!");
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

void Project::save() { m_database->serializeProject(); }

bool Project::close(bool promptSave) {
  if (promptSave) {
    // TODO: Implement when safe to do so
    if (m_database) {
      // m_database->gameConstants.serialize(m_database->basePath + "/data/Constants.json");
      // if (m_database->gameConstants.generateJS) {
      //   m_database->gameConstants.generateConstantsJS(m_database->basePath + "/js/Constants.js");
      // }
    }
  }

  SerializationQueue::instance().reset();
  DeserializationQueue::instance().reset();

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
                                  ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::Begin("##ore_toolbar", nullptr, window_flags);
  ImGui::PopStyleVar();

  if (ImGui::Button(ICON_FA_FILE, ButtonSize)) {
    handleCreateNewProject();
  }
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip("New Project", "Creates a new project.");
  }
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_FOLDER_OPEN, ButtonSize)) {
    handleOpenFile();
  }
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip("Open Project", "Opens an existing project.");
  }
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_FLOPPY_DISK, ButtonSize)) {
    save();
  }
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
    if (ImGui::Button(ICON_FA_CHESS_BOARD, ButtonSize)) {
      enterMapEditMode();
    }
  }
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip("Map", "Switch to Map Edit mode.");
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  ImGui::BeginDisabled(m_editMode == EditMode::Event);
  {
    if (ImGui::Button(ICON_FA_CHESS_PAWN, ButtonSize)) {
      enterEventEditMode();
    }
  }
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip("Event", "Switch to Event Edit mode.");
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  if (ImGui::Button(!m_mapEditor.prisonMode() ? ICON_FA_EYE : ICON_FA_EYE_SLASH, ButtonSize)) {
    m_mapEditor.setPrisonMode(!m_mapEditor.prisonMode());
  }
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip("Game Preview", "Toggles Game Preview mode.");
  }
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
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip("Shadow Pen", "Adds or removes shadows of walls");
  }
  ImGui::EndDisabled();
  ImGui::End();
}

void Project::draw() {
  m_toolbarSize = App::DPIHandler::scale_value(Settings::instance()->fontSize + 32);
  drawMenu();
  drawToolbar();
  setupDocking();
  m_mapEditor.draw();

  if (m_databaseEditor) {
    m_databaseEditor->draw();
  }

  if (m_editMode == EditMode::Map) {
    m_tilesetPicker.draw();
  } else {
    m_eventListView.draw();
  }

  m_mapListView.draw();
  drawTileDebugger();

  if (m_showDemoWindow) {
    ImGui::ShowDemoWindow(&m_showDemoWindow);
  }
  if (m_showAboutWindow) {
    ImGui::ShowAboutWindow(&m_showAboutWindow);
  }

  m_nwjsVersionManager.draw();

  if (DeserializationQueue::instance().hasTasks()) {
    ImGui::Begin("Loading Project....", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Loading %s (%i of %i)....", DeserializationQueue::instance().getCurrentFile().data(),
                DeserializationQueue::instance().currentTaskIndex(), DeserializationQueue::instance().totalTasks());
    ImGui::ProgressBar(DeserializationQueue::instance().getProgress() / 100.f);
    if (ImGui::Button("Cancel")) {
      DeserializationQueue::instance().reset();
      close();
    }
    ImGui::End();
  } else if (DeserializationQueue::instance().getProgress() >= 100.f) {
    DeserializationQueue::instance().reset();
  }
  if (SerializationQueue::instance().hasTasks()) {
    ImGui::Begin("Saving Project....", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Saving %s (%i of %i)....", SerializationQueue::instance().getCurrentFile().data(),
                SerializationQueue::instance().currentTaskIndex(), SerializationQueue::instance().totalTasks());
    ImGui::ProgressBar(SerializationQueue::instance().getProgress() / 100.f);
    if (ImGui::Button("Cancel")) {
      SerializationQueue::instance().reset();
      close();
    }
    ImGui::End();
  } else if (SerializationQueue::instance().getProgress() >= 100.f) {
    SerializationQueue::instance().reset();
  }

  ImGui::RenderNotifications();

  drawCreateNewProjectPopup();
}

void Project::drawCreateNewProjectPopup() {
  static constexpr std::array<std::string_view, 23> directoryList{{"data"sv,
                                                                   "fonts"sv,
                                                                   "audio/bgm"sv,
                                                                   "audio/bgs"sv,
                                                                   "audio/me"sv,
                                                                   "audio/se"sv,
                                                                   "img/animations"sv,
                                                                   "img/battlebacks1"sv,
                                                                   "img/battlebacks2"sv,
                                                                   "img/characters"sv,
                                                                   "img/enemies"sv,
                                                                   "img/faces"sv,
                                                                   "img/parallaxes"sv,
                                                                   "img/pictures"sv,
                                                                   "img/sv_actors"sv,
                                                                   "img/sv_enemies"sv,
                                                                   "img/system"sv,
                                                                   "img/tilesets"sv,
                                                                   "img/titles1"sv,
                                                                   "img/titles2"sv,
                                                                   "js/libs"sv,
                                                                   "js/plugins"sv,
                                                                   "movies"sv}};

  const auto [closed, confirmed] = m_createNewProject.draw();
  if (closed) {
    if (confirmed) {
      const auto projectName = m_createNewProject.projectName();
      const auto gameTitle = m_createNewProject.gameTitle();

      const auto projectPath = Settings::instance()->projectBaseDirectory / std::filesystem::path(projectName);

      if (create_directories(projectPath)) {
        for (const auto& directory : directoryList) {
          create_directories(projectPath / directory);
        }

        auto projectFilePath = projectPath / std::filesystem::path("Game.rpgproject");
        std::ofstream projectFile(projectFilePath);
        projectFile << KnownRPGMVVersions[KnownRPGMVVersions.size() - 2] << std::endl;
        SerializationQueue::instance().setBasepath(projectPath.generic_string());
        m_database.emplace(projectPath.generic_string(), projectFilePath.generic_string(),
                           KnownRPGMVVersions[KnownRPGMVVersions.size() - 2]);
        m_database->system.gameTitle = gameTitle;
        m_database->system.locale = "en_US";
        m_database->system.versionId = floor(rand() * 100000000);
        m_database->serializeProject();
        while (SerializationQueue::instance().hasTasks()) {}
        load(projectFilePath.generic_string(), projectPath.generic_string());
      }
    }
  }
}

void Project::drawTileInfo(MapRenderer::MapLayer& mapLayer, int z) {
  static constexpr std::array LayerNames{"A1"sv, "A2"sv, "A3"sv, "A4"sv, "A5"sv, "B"sv, "C"sv, "D"sv, "E"sv};
  for (const auto& layer : mapLayer.tileLayers) {
    for (const auto& tile : layer.rects) {
      if ((tile.x >= m_mapEditor.tileAbsoluteX() && tile.x < m_mapEditor.tileAbsoluteX() + m_mapEditor.tileSize()) &&
          (tile.y >= m_mapEditor.tileAbsoluteY() && tile.y < m_mapEditor.tileAbsoluteY() + m_mapEditor.tileSize())) {
        float u0 = tile.u;
        float v0 = tile.v;
        float u1 = u0 + tile.tileWidth;
        float v1 = v0 + tile.tileHeight;

        u0 /= layer.tex.width();
        v0 /= layer.tex.height();
        u1 /= layer.tex.width();
        v1 /= layer.tex.height();
        ImGui::Image(layer.tex,
                     {static_cast<float>(m_mapEditor.tileSize()), static_cast<float>(m_mapEditor.tileSize())},
                     ImVec2{u0, v0}, ImVec2{u1, v1});
        std::string info =
            std::format("Sheet: {} ({}), ID: {}, x: {}, y: {}, z: {},  width: {}, height: {}, u: {}, v: {}",
                        m_mapEditor.mapRenderer().tileset()->tilesetNames[tile.tileSheet], LayerNames[tile.tileSheet],
                        tile.tileId, tile.x, tile.y, z, tile.tileWidth, tile.tileHeight, tile.u, tile.v);
        if (TileHelper::isAutoTile(tile.tileId)) {
          info += std::format(
              "\n"
              "AutoTile Shape: {}, Kind: {}\n"
              "WaterTile: {}, WaterfallTile: {}, GroundTile {}, RoofTile: {}, ShadowingTile: {},\n"
              "WallSide: {}, WallTop: {}, FloorTypeAutoTile: {}, WallTypeAutoTile: {}\n",
              TileHelper::getAutoTileShape(tile.tileId), TileHelper::getAutoTileKind(tile.tileId),
              TileHelper::isWaterTile(tile.tileId), TileHelper::isWaterfallTile(tile.tileId),
              TileHelper::isGroundTile(tile.tileId), TileHelper::isRoofTile(tile.tileId),
              TileHelper::isShadowingTile(tile.tileId), TileHelper::isWallTopTile(tile.tileId),
              TileHelper::isWallSideTile(tile.tileId), TileHelper::isFloorTypeAutotile(tile.tileId),
              TileHelper::isWallTypeAutotile(tile.tileId));
        }
        ImGui::Text("%s", info.c_str());
      }
    }
  }
}

void Project::drawTileDebugger() {
  if (!m_showTileDebug || !m_mapEditor.map()) {
    return;
  }

  if (ImGui::Begin("Tile Debug", &m_showTileDebug)) {
    if (ImGui::BeginTabBar("##tiledebugger")) {
      if (ImGui::BeginTabItem("Lower")) {
        for (int z = 0; z < 6; ++z) {
          drawTileInfo(m_mapEditor.mapRenderer().m_lowerLayers[z], z);
        }
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Upper")) {
        for (int z = 0; z < 6; ++z) {
          drawTileInfo(m_mapEditor.mapRenderer().m_upperLayers[z], z);
        }
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
  }
  ImGui::End();
}

void Project::handleOpenFile() {
  nfdu8char_t* outPath;
  constexpr nfdu8filteritem_t filters = {"RPG Maker Projects", "rpgproject,rmmzproject"};
  const std::string directory = Settings::instance()->lastDirectory.empty()
                                    ? std::filesystem::current_path().generic_string()
                                    : Settings::instance()->lastDirectory;
  const auto result = NFD_OpenDialogU8(&outPath, &filters, 1, directory.c_str());
  if (result == NFD_OKAY) {
    const std::filesystem::path path{outPath};
    Settings::instance()->lastDirectory = absolute(path).remove_filename().generic_string();
    load(absolute(path).generic_string(), Settings::instance()->lastDirectory);
    NFD_FreePathU8(outPath);
  }
}

void Project::handleCreateNewProject() { m_createNewProject.SetOpen(true); }

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
        handleCreateNewProject();
      }
      if (ImGui::MenuItem(ICON_FA_FOLDER " Open Project...", "Ctrl+O")) {
        handleOpenFile();
      }
      if (ImGui::MenuItem(ICON_FA_FOLDER_CLOSED " Close Project...")) {
        close();
      }
      if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save Project...", "Ctlr+S")) {
        save();
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
      if (ImGui::MenuItem("Game Preview", "F7", !m_mapEditor.prisonMode())) {
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
      if (ImGui::MenuItem("NWJS Version Manager", "F10", false)) {
        m_nwjsVersionManager.open();
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
  if (ImGui::IsKeyReleased(ImGuiKey_F10)) {
    m_nwjsVersionManager.open();
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
    m_database->mapInfos.setCurrentMap(&in);
  } else {
    m_mapEditor.setMap(nullptr);
    m_database->mapInfos.setCurrentMap(nullptr);
  }

  m_database->system.editMapId = in.id;

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