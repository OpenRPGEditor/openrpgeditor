#include "Core/MainWindow.hpp"

#include "Settings.hpp"

#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Log.hpp"
#include "Core/SettingsDialog/GeneralSettingsTab.hpp"
#include "Core/Utils.hpp"
#include "Database/Serializable/DeserializationQueue.hpp"
#include "Database/Serializable/SerializationQueue.hpp"
#include "Database/Versions.hpp"
#include "IconsFontAwesome6.h"
#include "imgui.h"
#include "imgui_internal.h"

#if _WIN32
/* clang-format off */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <winuser.h>
/* clang-format on */
#endif

#include "EditorPlugin/EditorPluginManager.hpp"
#include "LCF_Importer/LCF_Importer.hpp"
#include "Script/ScriptEngine.hpp"
#include "SettingsDialog/RPGMakerLocationAndVersionTab.hpp"
#include "SettingsDialog/UISettingsTab.hpp"

#include <array>
#include <clip.h>
#include <fstream>
#include <nfd.h>
#include <string_view>

using namespace std::literals::string_view_literals;

MainWindow* MainWindow::m_instance = nullptr;
MainWindow::MainWindow() : m_mapListView(this), m_mapEditor(this), m_eventListView(this), m_tilesetPicker(this), m_nwjsVersionManager("https://dl.nwjs.io"), m_eventSearcher(this), m_libLCF(this) {
  m_settingsDialog.addTab(new GeneralSettingsTab());
  m_settingsDialog.addTab(new UISettingsTab());
  m_settingsDialog.addTab(new RPGMakerLocationAndVersionTab());
  m_instance = this;
}

MainWindow::ToolbarButton::ToolbarButton(const std::string& id, const ToolbarCategory category, asIScriptFunction* func) : m_id(id), m_category(category), m_func(func) {

  if (func && func->GetFuncType() == asFUNC_DELEGATE) {
    m_callbackObject = func->GetDelegateObject();
    m_callbackObjectType = func->GetDelegateObjectType();
    m_func = func->GetDelegateFunction();
    ScriptEngine::instance()->engine()->AddRefScriptObject(m_callbackObject, m_callbackObjectType);
    (void)m_func->AddRef();
    (void)func->Release();
  } else {
    m_func = func;
  }
}

MainWindow::ToolbarButton::~ToolbarButton() {
  if (m_func) {
    if (m_callbackObject && m_callbackObjectType) {
      ScriptEngine::instance()->engine()->ReleaseScriptObject(m_callbackObject, m_callbackObjectType);
    }
    (void)m_func->Release();
  }
}

void MainWindow::ToolbarButton::callOnClicked() const {
  if (!m_func) {
    return;
  }

  if (const auto ctx = ScriptEngine::instance()->prepareContextFromPool(m_func)) {
    if (m_callbackObject != nullptr) {
      ctx->SetObject(m_callbackObject);
    }
    ScriptEngine::executeCall(ctx);
    ScriptEngine::instance()->returnContextToPool(ctx);
  }
}

bool MainWindow::load(std::string_view filePath, std::string_view basePath) {
  if (!std::filesystem::exists(filePath)) {
    if (const auto it = std::ranges::find_if(Settings::instance()->mru, [&filePath](const auto& pair) { return pair.first == filePath; }); it != Settings::instance()->mru.end()) {
      ImGui::InsertNotification({ImGuiToastType::Error, trNOOP("Unable to open project:\n%s"), filePath.data()});
      Settings::instance()->mru.erase(it);
    }
    return false;
  }
  close();
  std::string version;
  std::ifstream file(filePath.data());
  std::getline(file, version);
  Utils::trim(version);

  bool isMZ = false;
  m_isValid = std::ranges::find(KnownRPGMVVersions, version) != KnownRPGMVVersions.end();
  if (!m_isValid) {
    m_isValid = std::ranges::find(KnownRPGMZVersions, version) != KnownRPGMZVersions.end();
    isMZ = true;
  }
  if (!m_isValid) {
    APP_ERROR("Invalid project version {}", version);
    return false;
  }
  APP_INFO("Got project for {}", version);
  SerializationQueue::instance().reset();
  DeserializationQueue::instance().reset();
  SerializationQueue::instance().setBasepath(basePath);
  DeserializationQueue::instance().setBasepath(basePath);
  ResourceManager::instance()->setBasepath(basePath);
  m_databaseEditor.emplace(this);
  m_databaseEditor->onReady.connect<&MainWindow::onDatabaseReady>(this);

  m_database.emplace(basePath, filePath, version, isMZ);
  m_database->actorsLoaded().connect<&MainWindow::onActorsLoaded>(this);
  m_database->classesLoaded().connect<&MainWindow::onClassesLoaded>(this);
  m_database->skillsLoaded().connect<&MainWindow::onSkillsLoaded>(this);
  m_database->itemsLoaded().connect<&MainWindow::onItemsLoaded>(this);
  m_database->weaponsLoaded().connect<&MainWindow::onWeaponsLoaded>(this);
  m_database->armorsLoaded().connect<&MainWindow::onArmorsLoaded>(this);
  m_database->enemiesLoaded().connect<&MainWindow::onEnemiesLoaded>(this);
  m_database->troopsLoaded().connect<&MainWindow::onTroopsLoaded>(this);
  m_database->statesLoaded().connect<&MainWindow::onStatesLoaded>(this);
  m_database->animationsLoaded().connect<&MainWindow::onAnimationsLoaded>(this);
  m_database->tilesetsLoaded().connect<&MainWindow::onTilesetsLoaded>(this);
  m_database->commonEventsLoaded().connect<&MainWindow::onCommonEventsLoaded>(this);
  m_database->systemLoaded().connect<&MainWindow::onSystemLoaded>(this);
  m_database->gameConstantsLoaded().connect<&MainWindow::onGameConstantsLoaded>(this);
  m_database->templatesLoaded().connect<&MainWindow::onTemplatesLoaded>(this);
  m_database->docsLoaded().connect<&MainWindow::onDocsLoaded>(this);
  m_database->localesLoaded().connect<&MainWindow::onLocalesLoaded>(this);
  m_database->load();
  return true;
}

void MainWindow::save() { m_database->serializeProject(); }

bool MainWindow::close(bool promptSave) {
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
  m_mapListView.reset();
  m_isLoaded = false;

  return true;
}

void MainWindow::setupDocking() {
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos + ImVec2(0, m_toolbarSize));
  ImGui::SetNextWindowSize(viewport->Size - ImVec2(0, m_toolbarSize));
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGuiWindowFlags window_flags = 0 | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

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
    ImGui::DockBuilderGetNode(dock3)->SetLocalFlags(static_cast<int>(ImGuiDockNodeFlags_NoUndocking) | static_cast<int>(ImGuiDockNodeFlags_NoDocking));
    // 7. We're done setting up our docking configuration:
    ImGui::DockBuilderFinish(mainWindowGroup);
  }
  ImGui::DockSpace(ImGui::GetID("MainWindowGroup"));
  ImGui::End();
  ImGui::PopStyleVar(3);
}

void MainWindow::drawToolbar() {
  const ImVec2 ButtonSize = {48, 48};
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + m_menuBarHeight));
  ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, m_toolbarSize));
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGuiWindowFlags window_flags = 0 | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                  ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::Begin("##ore_toolbar", nullptr, window_flags);
  ImGui::PopStyleVar();

  ORE_DISABLE_EXPERIMENTAL_BEGIN();
  if (ImGui::Button(ICON_FA_FILE, ButtonSize)) {
    handleCreateNewProject();
  }
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip(trNOOP("New Project"), trNOOP("Creates a new project. [EXPERIMENTAL]"));
  }
  ORE_DISABLE_EXPERIMENTAL_END();
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_FOLDER_OPEN, ButtonSize)) {
    handleOpenFile();
  }
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip(trNOOP("Open Project"), trNOOP("Opens an existing project."));
  }
  ImGui::SameLine();
  ORE_DISABLE_EXPERIMENTAL_BEGIN();
  if (ImGui::Button(ICON_FA_FLOPPY_DISK, ButtonSize)) {
    save();
  }
  ImGui::SameLine();
  if (ImGui::IsItemHovered()) {
    ImGui::ActionTooltip(trNOOP("Save Project"), trNOOP("Saves the project. [EXPERIMENTAL]"));
  }
  ORE_DISABLE_EXPERIMENTAL_END();

  ORE_CHECK_EXPERIMENTAL_BEGIN()
  for (const auto& button : m_toolbarButtons[ToolbarCategory::File]) {
    ImGui::SameLine();
    if (ImGui::Button(button.id().c_str(), ButtonSize)) {
      button.callOnClicked();
    }
  }
  ORE_CHECK_EXPERIMENTAL_END()

  ImGui::SameLine();
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
    if (ImGui::ImageButton("map_button", ResourceManager::instance()->loadEditorTexture(std::format("icons/map/map_128x128.png")), ButtonSize)) {
      enterMapEditMode();
    }
  }
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip(trNOOP("Map"), trNOOP("Switch to Map Edit mode."));
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  ImGui::BeginDisabled(m_editMode == EditMode::Event);
  {
    if (ImGui::ImageButton("event_button", ResourceManager::instance()->loadEditorTexture(std::format("icons/map/event_128x128.png")), ButtonSize)) {
      enterEventEditMode();
    }
  }
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip(trNOOP("Event"), trNOOP("Switch to Event Edit mode."));
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  if (ImGui::ImageButton("preview_button", ResourceManager::instance()->loadEditorTexture(std::format("icons/map/preview{}_128x128.png", !m_mapEditor.prisonMode() ? "-off" : "")), ButtonSize)) {
    m_mapEditor.setPrisonMode(!m_mapEditor.prisonMode());
  }
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip(trNOOP("Game Preview"), trNOOP("Toggles Game Preview mode."));
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
    ImGui::ActionTooltip(trNOOP("Shadow Pen"), trNOOP("Adds or removes shadows of walls"));
  }
  ImGui::EndDisabled();
  ImGui::SameLine();
  ImGui::BeginDisabled(editMode() != EditMode::Map || m_drawTool == DrawTool::Eraser);
  {
    if (ImGui::Button(ICON_FA_ERASER, ButtonSize)) {
      setDrawTool(DrawTool::Eraser);
    }
  }
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::ActionTooltip(trNOOP("Eraser"), trNOOP("Erases tiles at a given point"));
  }
  ImGui::EndDisabled();
  ImGui::End();
}

void MainWindow::draw() {
  m_toolbarButtonSize = 64;
  m_toolbarSize = m_toolbarButtonSize + (ImGui::GetStyle().ItemSpacing.y * 4) + (ImGui::GetStyle().FramePadding.y * 4);
  m_toolbarButtonSize += ImGui::GetStyle().ItemSpacing.x * 2;
  drawMenu();
  drawToolbar();
  setupDocking();
  m_settingsDialog.draw();
  m_mapEditor.draw();
  m_eventSearcher.draw();
  m_libLCF.draw();

  if (m_databaseEditor) {
    m_databaseEditor->draw();
  }

  if (m_editMode == EditMode::Map) {
    m_tilesetPicker.draw();
  } else {
    m_eventListView.draw();
  }

  m_mapListView.draw();

  if (const auto [closed, confirmed] = m_mapProperties.draw(); closed) {
    // TODO: handle revert?
  }
  ORE_CHECK_DEBUG_BEGIN()

  drawTileDebugger();

  if (m_showDemoWindow) {
    ImGui::ShowDemoWindow(&m_showDemoWindow);
  }
  ORE_CHECK_DEBUG_END()

  if (m_showAboutWindow) {
    ImGui::ShowAboutWindow(&m_showAboutWindow);
  }

  m_nwjsVersionManager.draw();
  EditorPluginManager::instance()->draw();

  if (DeserializationQueue::instance().hasTasks()) {
    ImGui::Begin(trNOOP("Loading Project...."), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("%s", trFormat("Loading {0} ({1} of {2})....", DeserializationQueue::instance().getCurrentFile().data(), DeserializationQueue::instance().currentTaskIndex(),
                               DeserializationQueue::instance().totalTasks())
                          .c_str());
    ImGui::ProgressBar(DeserializationQueue::instance().getProgress() / 100.f);
    if (ImGui::Button(trNOOP("Cancel"))) {
      DeserializationQueue::instance().reset();
      close();
    }
    ImGui::End();
  } else if (DeserializationQueue::instance().getProgress() >= 100.f) {
    DeserializationQueue::instance().reset();
  }
  if (SerializationQueue::instance().hasTasks()) {
    ImGui::Begin(trNOOP("Saving Project...."), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("%s", trFormat("Saving {0} ({1} of {2})....", SerializationQueue::instance().getCurrentFile().data(), SerializationQueue::instance().currentTaskIndex(),
                               SerializationQueue::instance().totalTasks())
                          .c_str());
    ImGui::ProgressBar(SerializationQueue::instance().getProgress() / 100.f);
    if (ImGui::Button(trNOOP("Cancel"))) {
      SerializationQueue::instance().reset();
      close();
    }
    ImGui::End();
  } else if (SerializationQueue::instance().getProgress() >= 100.f) {
    SerializationQueue::instance().reset();
  }

  ImGui::RenderNotifications();

  drawCreateNewProjectPopup();

  EditorPluginManager::instance()->callDraws();
}

void MainWindow::drawCreateNewProjectPopup() {
  static constexpr std::array<std::string_view, 23> directoryList{{
      "data"sv,           "fonts"sv,       "audio/bgm"sv, "audio/bgs"sv,      "audio/me"sv,     "audio/se"sv,      "img/animations"sv, "img/battlebacks1"sv, "img/battlebacks2"sv,
      "img/characters"sv, "img/enemies"sv, "img/faces"sv, "img/parallaxes"sv, "img/pictures"sv, "img/sv_actors"sv, "img/sv_enemies"sv, "img/system"sv,       "img/tilesets"sv,
      "img/titles1"sv,    "img/titles2"sv, "js/libs"sv,   "js/plugins"sv,     "movies"sv,
  }};

  const auto [closed, confirmed] = m_createNewProject.draw();
  if (closed) {
    if (confirmed) {
      // TODO: Add RPGMaker MZ example copy
      const auto isMZ = m_createNewProject.projectType() == CreateNewProjectDialog::ProjectType::RPGMZ;
      const auto projectName = m_createNewProject.projectName();
      const auto gameTitle = m_createNewProject.gameTitle();
      const auto copyExampleProject = m_createNewProject.copyExample();

      const auto projectPath = Settings::instance()->projectBaseDirectory / std::filesystem::path(projectName);

      if (create_directories(projectPath)) {
        for (const auto& directory : directoryList) {
          create_directories(projectPath / directory);
        }

        auto projectFilePath = projectPath / (isMZ ? std::filesystem::path("Game.rmmzproject") : std::filesystem::path("Game.rpgproject"));
        std::ofstream projectFile(projectFilePath);
        std::string version;
        if (isMZ) {
          version = Settings::instance()->rpgMakerMZVersion != -1 ? KnownRPGMZVersions[Settings::instance()->rpgMakerMZVersion] : KnownRPGMZVersions[KnownRPGMZVersions.size() - 1];
        } else {
          version = Settings::instance()->rpgMakerMVVersion != -1 ? KnownRPGMVVersions[Settings::instance()->rpgMakerMVVersion] : KnownRPGMVVersions[KnownRPGMVVersions.size() - 1];
        }
        projectFile << version << std::endl;
        SerializationQueue::instance().setBasepath(projectPath.generic_string());
        if (copyExampleProject) {
          // TODO: Locale
          std::filesystem::path examplePath;
          if (!isMZ) {
            std::filesystem::path(Settings::instance()->rpgMakerMVLocation) / "NewData";
          } else {
            // This newdata needs to be lowercase when copying an RPG Maker MZ example project
            std::filesystem::path(Settings::instance()->rpgMakerMZLocation) / "newdata";
          }
          // TODO: MZ corescripts selection

          examplePath.make_preferred();
          if (exists(examplePath)) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(examplePath)) {
              const auto tmpPath = entry.path().generic_string().substr(examplePath.generic_string().size() + 1);
              const auto destPath = projectPath / std::filesystem::path(tmpPath);
              if (entry.is_directory() && !exists(destPath)) {
                create_directories(destPath);
              } else if (entry.is_regular_file()) {
                std::cout << destPath.generic_string() << std::endl;
                std::filesystem::copy_file(entry.path(), destPath);
              }
            }
          }
        }
        if (!copyExampleProject) {
          m_database.emplace(projectPath.generic_string(), projectFilePath.generic_string(), version, isMZ);
          m_database->system.setGameTitle(gameTitle.data());
          m_database->system.setLocale("en_US");
          m_database->system.setVersionId(floor(rand() * 100000000));
          m_database->serializeProject();
        }
        while (SerializationQueue::instance().hasTasks()) {}
        load(projectFilePath.generic_string(), projectPath.generic_string());
      }
    }
  }
}

void MainWindow::drawTileInfo(MapRenderer::MapLayer& mapLayer, int z) {
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
        ImGui::Image(layer.tex, {static_cast<float>(m_mapEditor.tileSize()), static_cast<float>(m_mapEditor.tileSize())}, ImVec2{u0, v0}, ImVec2{u1, v1});
        std::string info = trFormat("Sheet: {} ({}), ID: {}, x: {}, y: {}, z: {},  width: {}, height: {}, u: {}, v: {}", m_mapEditor.mapRenderer().tileset()->tilesetName(tile.tileSheet),
                                    LayerNames[tile.tileSheet], tile.tileId, tile.x, tile.y, z, tile.tileWidth, tile.tileHeight, tile.u, tile.v);
        if (TileHelper::isAutoTile(tile.tileId)) {
          info += trFormat(
              "\n"
              "AutoTile Shape: {}, Kind: {}\n"
              "WaterTile: {}, WaterfallTile: {}, GroundTile {}, RoofTile: {}, ShadowingTile: {},\n"
              "WallSide: {}, WallTop: {}, FloorTypeAutoTile: {}, WallTypeAutoTile: {}\n",
              TileHelper::getAutoTileShape(tile.tileId), TileHelper::getAutoTileKind(tile.tileId), TileHelper::isWaterTile(tile.tileId), TileHelper::isWaterfallTile(tile.tileId),
              TileHelper::isGroundTile(tile.tileId), TileHelper::isRoofTile(tile.tileId), TileHelper::isShadowingTile(tile.tileId), TileHelper::isWallTopTile(tile.tileId),
              TileHelper::isWallSideTile(tile.tileId), TileHelper::isFloorTypeAutotile(tile.tileId), TileHelper::isWallTypeAutotile(tile.tileId));
        }
        ImGui::Text("%s", info.c_str());
      }
    }
  }
}

void MainWindow::drawTileDebugger() {
  if (!m_showTileDebug || !m_mapEditor.map()) {
    return;
  }

  if (ImGui::Begin(trNOOP("Tile Debug"), &m_showTileDebug)) {
    if (ImGui::BeginTabBar("##tiledebugger")) {
      if (ImGui::BeginTabItem(trNOOP("Lower"))) {
        for (int z = 0; z < 6; ++z) {
          drawTileInfo(m_mapEditor.mapRenderer().m_lowerLayers[z], z);
        }
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem(trNOOP("Upper"))) {
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

void MainWindow::handleOpenFile() {
  nfdu8char_t* outPath;
  const nfdu8filteritem_t filters = {trNOOP("RPG Maker Projects"), "rpgproject,rmmzproject"};
  const std::string directory = Settings::instance()->lastDirectory.empty() ? std::filesystem::current_path().generic_string() : Settings::instance()->lastDirectory;
  const auto result = NFD_OpenDialogU8(&outPath, &filters, 1, directory.c_str());
  if (result == NFD_OKAY) {
    const std::filesystem::path path{outPath};
    Settings::instance()->lastDirectory = absolute(path).remove_filename().generic_string();
    load(absolute(path).generic_string(), Settings::instance()->lastDirectory);
    NFD_FreePathU8(outPath);
  }
}

void MainWindow::handleCreateNewProject() { m_createNewProject.setOpen(true); }

void MainWindow::handleUndo() {
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

void MainWindow::handleRedo() {
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
void MainWindow::drawMenu() {
  std::string loadFilepath;
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu(trNOOP("File"))) {
      if (ImGui::MenuItem(std::format("{} {}", ICON_FA_FILE, tr("New Project...")).c_str(), "Ctrl+N")) {
        handleCreateNewProject();
      }
      if (ImGui::MenuItem(std::format("{} {}", ICON_FA_FOLDER, tr("Open Project...")).c_str(), "Ctrl+O")) {
        handleOpenFile();
      }
      if (ImGui::MenuItem(std::format("{} {}", ICON_FA_FOLDER_CLOSED, tr("Close Project...")).c_str())) {
        close();
      }
      if (ImGui::MenuItem(std::format("{} {}", ICON_FA_FLOPPY_DISK, tr("Save Project...")).c_str(), "Ctlr+S")) {
        save();
      }
      ImGui::Separator();
      if (ImGui::BeginMenu(trNOOP("Recent Projects"), !Settings::instance()->mru.empty())) {
        if (ImGui::MenuItem(std::format("{} {}", ICON_FA_BRUSH, tr("Clear")).c_str())) {
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
      // TL-NOTE: Use ellipses character appropriate for your language
      // Deployment is referring to preparing a game for distribution to end users, exporting all related assets into an easily distributable form
      if (ImGui::MenuItem(std::format("{} {}", ICON_FA_BOXES_PACKING, tr("Deployment...")).c_str(), " ")) {
        // TODO: Implement game deployment
      }
      /*
      if (ImGui::MenuItem("Steam Management")) {
        // TODO: Debate on implementing this, it may not be wise, or worthwhile
      }
      */
      ImGui::Separator();
      // We're using std::format here instead of trFORMAT because we don't want to risk someone localizing the icon after the text
      if (ImGui::MenuItem(std::format("{} {}", ICON_FA_DOOR_CLOSED, tr("Exit")).c_str(), "Ctrl+Q")) {
        App::APP->stop();
      }
      ImGui::EndMenu();
    }

    /* TODO: Implement shortcut handling, ImGui doesn't handle that for various reasons
     * so we have to do that ourselves
     */
    if (ImGui::BeginMenu(trNOOP("Edit"))) {
      if (ImGui::MenuItem(trNOOP("Undo"), "Ctrl+Z", false, m_undoStack.hasCommands())) {
        handleUndo();
      }
      if (ImGui::IsItemHovered() && m_undoStack.hasCommands()) {
        ImGui::SetTooltip("%s", m_undoStack.top()->description().c_str());
      }

      if (ImGui::MenuItem(trNOOP("Redo"), "Ctrl+Shift+Z", false, m_redoStack.hasCommands())) {
        handleRedo();
      }
      if (ImGui::IsItemHovered() && m_redoStack.hasCommands()) {
        ImGui::SetTooltip("%s", m_redoStack.top()->description().c_str());
      }
      ImGui::Separator();
      if (ImGui::MenuItem(trNOOP("Cut"), "Ctrl+X")) {
        // TODO: Implement clipboard
      }
      if (ImGui::MenuItem(trNOOP("Copy"), "Ctrl+C")) {
        // TODO: Implement clipboard
      }
      if (ImGui::MenuItem(trNOOP("Paste"), "Ctrl+V")) {
        // TODO: Implement clipboard
      }
      if (ImGui::MenuItem(trNOOP("Delete"), "Del")) {
        // TODO: Implement
      }
      ImGui::Separator();
      if (ImGui::MenuItem(trNOOP("Find"), "Ctrl+F")) {
        // TODO: Implement
      }
      if (ImGui::MenuItem(trNOOP("Find Next"), "F3")) {
        // TODO: Implement
      }
      if (ImGui::MenuItem(trNOOP("Find Previous"), "Shift+F3")) {
        // TODO: Implement
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(trNOOP("Mode"))) {
      if (ImGui::MenuItem(trNOOP("Map"), "F5", editMode() == EditMode::Map)) {
        enterMapEditMode();
      }
      if (ImGui::MenuItem(trNOOP("Event"), "F6", editMode() == EditMode::Event)) {
        enterEventEditMode();
      }
      ImGui::Separator();
      if (ImGui::MenuItem(trNOOP("Game Preview"), "F7", !m_mapEditor.prisonMode())) {
        m_mapEditor.togglePrisonMode();
      }
      if (ImGui::IsItemHovered()) {
        // TL-NOTE: Refers to Game Preview mode and is in reference to how events are drawn to the screen.
        // Displays as a tooltip.
        ImGui::SetTooltip("%s", trNOOP("Toggles event borders, layer render order, and animations."));
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(trNOOP("Draw"))) {
      for (auto v : magic_enum::enum_values<DrawTool>()) {
        if (ImGui::MenuItem(DecodeEnumName(magic_enum::enum_name(v)).data(), nullptr, drawTool() == v, editMode() == EditMode::Map)) {
          setDrawTool(v);
        }
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(trNOOP("Scale"))) {
      if (ImGui::MenuItem(trNOOP("Zoom In"), "Ctrl++", false, m_mapEditor.zoom() > 0.25f)) {
        m_mapEditor.scale(0.25f);
      }
      if (ImGui::MenuItem(trNOOP("Zoom Out"), "Ctrl+-", false, m_mapEditor.zoom() < 4.f)) {
        m_mapEditor.scale(-0.25f);
      }
      if (ImGui::MenuItem(trNOOP("Actual Size"), "Ctrl+0", false, m_mapEditor.zoom() != 1.f)) {
        m_mapEditor.setScale(1.f);
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(trNOOP("Tools"))) {
      if (ImGui::MenuItem(trNOOP("Settings"), "F8")) {
        m_settingsDialog.setOpen(true);
      }
      if (ImGui::MenuItem(trNOOP("Database"), "F9", false, m_databaseEditor != std::nullopt && m_databaseEditor->isReady())) {
        m_databaseEditor->open();
      }
      if (ImGui::MenuItem(trNOOP("NWJS Version Manager"), "F10")) {
        m_nwjsVersionManager.open();
      }
      if (ImGui::MenuItem(trNOOP("Event Searcher..."), "F11", false, m_databaseEditor != std::nullopt && m_databaseEditor->isReady())) {
        m_eventSearcher.open();
      }
      if (ImGui::MenuItem(trNOOP("Editor Plugins..."), "F12")) {
        EditorPluginManager::instance()->setOpen(true);
      }
      if (ImGui::MenuItem(trNOOP("LibLCF..."), "", false, m_databaseEditor != std::nullopt && m_databaseEditor->isReady())) {
        m_libLCF.open();
      }

      /* Add tools above this */
      ImGui::Separator();
      if (ImGui::MenuItem(trNOOP("Reset Window Layout"))) {
        ImGui::ClearIniSettings();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(trNOOP("Game"))) {
      if (ImGui::MenuItem(trNOOP("Play Test"), "Ctrl+R", false, m_databaseEditor != std::nullopt)) {
        // TODO: Implement
      }
      ImGui::Separator();
      if (ImGui::MenuItem(trNOOP("Open Folder"), nullptr, false, m_databaseEditor != std::nullopt)) {
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
      ORE_CHECK_DEBUG_BEGIN()
      if (ImGui::MenuItem("Show Tile Debugger", nullptr, &m_showTileDebug)) {}
      if (ImGui::MenuItem("Show Demo", nullptr, &m_showDemoWindow)) {}
      ORE_CHECK_DEBUG_END()
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  if (!loadFilepath.empty()) {
    load(loadFilepath, std::filesystem::absolute(loadFilepath).remove_filename().generic_string());
  }
  handleKeyboardShortcuts();
}

void MainWindow::handleKeyboardShortcuts() {
  /* File Menu */
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_N)) {
    // TODO: implement new project creation
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_O)) {
    handleOpenFile();
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_S)) {
    // TODO: implement saving
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_Q)) {
    App::APP->stop();
  }

  /* Edit Menu */
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && !(ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) &&
      ImGui::IsKeyPressed(ImGuiKey_Z)) {
    handleUndo();
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) &&
      ImGui::IsKeyPressed(ImGuiKey_Z)) {
    handleRedo();
  }
  /* TODO Clipboard */
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_X)) {
    // TODO: Implement clipboard cut
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_C)) {
    // TODO: Implement clipboard copy
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_V)) {
    // TODO: Implement clipboard paste
  }

  if (ImGui::IsKeyReleased(ImGuiKey_Delete)) {
    // TODO: Implement Delete
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_F)) {
    // TODO: Implement Find
  }
  if (ImGui::IsKeyPressed(ImGuiKey_F3)) {
    // TODO: Implement Find Next
  }

  if ((ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) && ImGui::IsKeyPressed(ImGuiKey_F3)) {
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
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && (ImGui::IsKeyPressed(ImGuiKey_Equal) || ImGui::IsKeyPressed(ImGuiKey_KeypadAdd))) {
    m_mapEditor.scale(0.25f);
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && (ImGui::IsKeyPressed(ImGuiKey_Minus) || ImGui::IsKeyPressed(ImGuiKey_KeypadSubtract))) {
    m_mapEditor.scale(-0.25f);
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && (ImGui::IsKeyPressed(ImGuiKey_Keypad0) || ImGui::IsKeyPressed(ImGuiKey_0))) {
    m_mapEditor.setScale(1.f);
  }

  /* Tools */
  if (ImGui::IsKeyReleased(ImGuiKey_F8)) {
    m_settingsDialog.setOpen(true);
  }
  if (ImGui::IsKeyReleased(ImGuiKey_F9)) {
    m_databaseEditor->open();
  }
  if (ImGui::IsKeyReleased(ImGuiKey_F10)) {
    m_nwjsVersionManager.open();
  }
  if (ImGui::IsKeyReleased(ImGuiKey_F11)) {
    m_eventSearcher.open();
  }
  if (ImGui::IsKeyReleased(ImGuiKey_F12)) {
    EditorPluginManager::instance()->setOpen();
  }

  // TODO: Add missing tools
  /* Game */
  if ((ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) && ImGui::IsKeyPressed(ImGuiKey_R)) {
    // TODO: Implement Play Test
  }

  /* Help */
  if (ImGui::IsKeyPressed(ImGuiKey_F1)) {
    // TODO: Help
  }
}

void MainWindow::setMap(MapInfo& in) {
  if (in.id() == 0) {
    m_mapListView.setCurrentMapId(0);
    m_mapEditor.setMap(nullptr);
    return;
  }

  m_mapListView.setCurrentMapId(in.id(), true);
  if (m_mapListView.currentMapInfo()) {
    m_mapEditor.setMap(&in);
    m_database->mapInfos.setCurrentMap(&in);
  } else {
    m_mapEditor.setMap(nullptr);
    m_database->mapInfos.setCurrentMap(nullptr);
  }

  m_database->system.setEditMapId(in.id());

  SDL_SetCursor(SDL_GetDefaultCursor());
}

void MainWindow::onActorsLoaded() { m_databaseEditor->setActors(m_database->actors); }
void MainWindow::onClassesLoaded() { m_databaseEditor->setClasses(m_database->classes); }
void MainWindow::onSkillsLoaded() { m_databaseEditor->setSkills(m_database->skills); }
void MainWindow::onItemsLoaded() { m_databaseEditor->setItems(m_database->items); }
void MainWindow::onWeaponsLoaded() { m_databaseEditor->setWeapons(m_database->weapons); }
void MainWindow::onArmorsLoaded() { m_databaseEditor->setArmors(m_database->armors); }
void MainWindow::onEnemiesLoaded() { m_databaseEditor->setEnemies(m_database->enemies); }
void MainWindow::onTroopsLoaded() { m_databaseEditor->setTroops(m_database->troops); }
void MainWindow::onStatesLoaded() { m_databaseEditor->setStates(m_database->states); }
void MainWindow::onAnimationsLoaded() { m_databaseEditor->setAnimations(m_database->animations); }
void MainWindow::onTilesetsLoaded() { m_databaseEditor->setTilesets(m_database->tilesets); }
void MainWindow::onCommonEventsLoaded() { m_databaseEditor->setCommonEvents(m_database->commonEvents); }
void MainWindow::onSystemLoaded() { m_databaseEditor->setSystem(m_database->system); }
void MainWindow::onGameConstantsLoaded() { m_databaseEditor->setGameConstants(m_database->gameConstants); }
void MainWindow::onTemplatesLoaded() { m_databaseEditor->setTemplates(m_database->templates); }
void MainWindow::onDocsLoaded() { m_databaseEditor->setDocs(m_database->docs); }
void MainWindow::onLocalesLoaded() { m_databaseEditor->setLocales(m_database->locales); }

void MainWindow::onDatabaseReady() {
  MapInfo* info = m_database->mapInfos.map(0);
  Settings::instance()->lastProject = m_database->projectFilePath;
  info->setExpanded(true);
  info->setName(m_database->system.gameTitle().empty() ? std::filesystem::path(m_database->projectFilePath).filename().generic_string() : m_database->system.gameTitle());
  APP_INFO("Loaded project!");
  // Load the previously loaded map
  m_mapEditor.clearLayers();
  m_mapListView.setMapInfos(&m_database->mapInfos);

  if (MapInfo* m = m_database->mapInfos.map(m_database->system.editMapId()); m != nullptr) {
    setMap(*m);
  }

  if (const auto fileIt = std::ranges::find_if(Settings::instance()->mru, [this](const auto& t) { return t.first == m_database->projectFilePath; }); fileIt == Settings::instance()->mru.end()) {
    Settings::instance()->mru.emplace_front(m_database->projectFilePath,
                                            m_database->system.gameTitle().empty() ? std::filesystem::path(m_database->projectFilePath).filename().generic_string() : m_database->system.gameTitle());
  } else {
    Settings::instance()->mru.erase(fileIt);
    Settings::instance()->mru.emplace_front(m_database->projectFilePath,
                                            m_database->system.gameTitle().empty() ? std::filesystem::path(m_database->projectFilePath).filename().generic_string() : m_database->system.gameTitle());
  }
  while (Settings::instance()->mru.size() > Settings::instance()->maxMru) {
    Settings::instance()->mru.pop_back();
  }
  m_isLoaded = true;
  /* Disconnect the signal so we don't get spammed */
  m_databaseEditor->onReady.disconnect<&MainWindow::onDatabaseReady>(this);
}

void MainWindow::addToolbarButton(const std::string& id, ToolbarCategory category, asIScriptFunction* func) { m_toolbarButtons[category].emplace_back(id, category, func); }

void AddToolbarButton(asIScriptGeneric* generic) {
  const std::string name = *static_cast<std::string*>(generic->GetArgAddress(0));
  const auto category = static_cast<ToolbarCategory>(generic->GetArgDWord(1));
  const auto func = static_cast<asIScriptFunction*>(generic->GetArgAddress(2));
  MainWindow::instance()->addToolbarButton(name, category, func);
}

void AddCallback(asIScriptFunction* func) {
  //
}
void MainWindow::RegisterBindings() {
  auto engine = ScriptEngine::instance()->engine();
  engine->RegisterEnum("ToolbarCategory");

  int r;
  for (const auto& category : magic_enum::enum_values<ToolbarCategory>()) {
    r = engine->RegisterEnumValue("ToolbarCategory", magic_enum::enum_name(category).data(), static_cast<int>(category));
    assert(r >= 0);
  }
  r = engine->RegisterFuncdef("void ToolBarButtonClicked()");
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void addToolbarButton(const string& in, ToolbarCategory category, ToolBarButtonClicked @const cb)", asFUNCTION(AddToolbarButton), asCALL_GENERIC);
  assert(r >= 0);
}