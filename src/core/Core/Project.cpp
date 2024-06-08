#include "Core/Project.hpp"

#include "ImGuiFileDialog.h"
#include "Core/ResourceManager.hpp"

#include "Core/Log.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiUtils.hpp"
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
    //"RPGMV 1.0.0"sv,
    "RPGMV 1.6.1"sv,
    "RPGMV 1.6.2"sv,
    "RPGMV 1.6.3"sv,
};

bool Project::load(std::string_view filePath, std::string_view basePath) {
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
  m_databaseEditor.emplace(m_actors, m_classes, m_skills, m_items, m_weapons, m_armors, m_enemies, m_troops, m_states,
                           m_animations, m_tilesets, m_commonEvents, m_system);
  MapInfo* info = m_mapInfos.map(0);
  info->expanded = true;
  info->name = m_system.gameTitle;
  APP_INFO("Loaded project!");
  m_resourceManager.emplace(m_basePath);
  // Load the previously loaded map
  SDL_SetCursor(SDL_GetDefaultCursor());
  MapInfo* m = m_mapInfos.map(m_system.editMapId);
  if (m != nullptr) {
    doMapSelection(*m);
  }
  return true;
}

bool Project::close(bool save) {
  if (save) {
    // TODO: Implement when safe to do so
  }

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
  m_selectedMapId = -1;

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
  drawMapEditor();

  if (m_editMode == EditMode::Map) {
    drawTilesets();
  } else {
    drawEventList();
  }

  drawMapTree();
  if (m_showDemoWindow) {
    ImGui::ShowDemoWindow(&m_showDemoWindow);
  }
  if (m_showAboutWindow) {
    ImGui::ShowAboutWindow(&m_showAboutWindow);
  }
}

void Project::drawMenu() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New Project...", "Ctrl+N")) {
        // TODO: Implement project creation
      }
      if (ImGui::MenuItem("Open Project...", "Ctrl+O")) {
        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog("OpenProjectDlg", "Select a Project to Open", ".rpgproject", config);
      }
      if (ImGui::MenuItem("Close Project...")) {
        close();
      }
      if (ImGui::MenuItem("Save Project...", "Ctlr+S")) {
        // TODO: Implement project saving
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
      if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
        // TODO: Implement undo/redo stack
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
      if (ImGui::MenuItem("Find Next", "Ctrl+G")) {
        // TODO: Implement
      }
      if (ImGui::MenuItem("Find Previous", "Ctrl+Shift+G")) {
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
        // TODO: Implement
      }
      if (ImGui::MenuItem("Zoom Out", "Ctrl+-")) {
        // TODO: Implement
      }
      if (ImGui::MenuItem("Actual Size", "Ctrl+0")) {
        // TODO: Implement
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
#if _APPLE__
        char buff[4096]{};
        snprintf(buff, 4096, "explorer %s", m_basePath.c_str());
#elif _WIN32_
        char buff[4096]{};
        snprintf(buff, 4096, "open %s", m_basePath.c_str());
#else
        char buff[4096]{};
        snprintf(buff, 4096, "xdg-open %s", m_basePath.c_str());
#endif
        if (strlen(buff) > 0) {
          system(buff);
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
}
void Project::drawFileDialog() {
  // First check if we have a pending project request
  if (ImGuiFileDialog::Instance()->Display("OpenProjectDlg",
                                           ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_Modal,
                                           ImVec2(600, 600))) {
    if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      load(filePathName, filePath);
      // action
    }

    // close
    ImGuiFileDialog::Instance()->Close();
  }
}

void Project::drawTilesets() {
  if (ImGui::Begin("Tilesets")) {
    if (ImGui::BeginTabBar("##tileset")) {
      if (ImGui::BeginTabItem("A", nullptr)) {
        if (m_map) {
          Texture tilesetTxtr =
              m_resourceManager->loadTilesetImage(m_tilesets.tileset(m_map->tilesetId)->tilesetNames[0]);
          if (tilesetTxtr) {
            ImGui::Image(tilesetTxtr.get(),
                         ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          }
        }
        ImGui::EndTabItem();
      }
      if (m_map && !m_tilesets.tileset(m_map->tilesetId)->tilesetNames[5].empty()) {
        if (ImGui::BeginTabItem("B", nullptr)) {
          Texture tilesetTxtr =
              m_resourceManager->loadTilesetImage(m_tilesets.tileset(m_map->tilesetId)->tilesetNames[5]);
          if (tilesetTxtr) {
            ImGui::Image(tilesetTxtr.get(),
                         ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          }
          ImGui::EndTabItem();
        }
      }
      if (m_map && !m_tilesets.tileset(m_map->tilesetId)->tilesetNames[6].empty()) {
        if (ImGui::BeginTabItem("C", nullptr)) {
          Texture tilesetTxtr =
              m_resourceManager->loadTilesetImage(m_tilesets.tileset(m_map->tilesetId)->tilesetNames[6]);
          if (tilesetTxtr) {
            ImGui::Image(tilesetTxtr.get(),
                         ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          }
          ImGui::EndTabItem();
        }
      }
      if (m_map && !m_tilesets.tileset(m_map->tilesetId)->tilesetNames[7].empty()) {
        if (ImGui::BeginTabItem("D", nullptr)) {
          Texture tilesetTxtr =
              m_resourceManager->loadTilesetImage(m_tilesets.tileset(m_map->tilesetId)->tilesetNames[7]);
          if (tilesetTxtr) {
            ImGui::Image(tilesetTxtr.get(),
                         ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          }
          ImGui::EndTabItem();
        }
      }
      // TODO: This needs to be either generated or made from scratch.
      if (ImGui::BeginTabItem("R", nullptr)) {
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
  }
  ImGui::End();
}

void Project::drawEventList() {
  char eventNameBuf[4096]{};
  if (ImGui::Begin("Events")) {
    if (m_map) {
      for (const auto& event : m_map->events) {
        if (event) {
          sprintf(eventNameBuf, "%s (%i, %i)", event->name.c_str(), event->x, event->y);
          ImGui::Selectable(eventNameBuf);
        }
      }
    }
  }
  ImGui::End();
}

void Project::doMapSelection(MapInfo& in) {
  if (in.id == 0) {
    m_map.reset();
    m_selectedMapId = 0;
    // m_mapRenderer.setMap(nullptr, nullptr);
    return;
  }
  SDL_SetCursor(waitCursor);
  m_selectedMapId = in.id;
  m_map = m_resourceManager->loadMap(in.id);
  // m_mapRenderer.setMap(&*m_map, m_tilesets.tileset(m_map->tilesetId));
  MapInfo* m = m_mapInfos.map(m_selectedMapId);
  if (m != nullptr) {
    m_initialScrollX = m->scrollX;
    m_initialScrollY = m->scrollX;
    m_initialScrollSet = true;
  }

#if 0
  printf("%zu bytes, %i w %i h\n", m_map->data.size(), m_map->width, m_map->height);
  for (int z = 0; z < 6; z++) {
    printf("-----------------Map Layer %.4i-----------------\n", z);
    for (int y = 0; y < m_map->height; y++) {
      for (int x = 0; x < m_map->width; x++) {
        int tileId = (z * m_map->height + y) * m_map->width + x;
        printf("0x%.04x ", m_tilesets.tileset(m_map->tilesetId)->flags[m_map->data[tileId]]);
      }
      printf("\n");
    }
  }
#endif
  SDL_SetCursor(SDL_GetDefaultCursor());
}

void Project::recursiveDrawTree(MapInfo& in) {
  ImGui::PushID(&in);

  if (ImGui::TreeNodeEx(&in,
                        (in.expanded ? ImGuiTreeNodeFlags_DefaultOpen : 0) |
                            (m_selectedMapId == in.id ? ImGuiTreeNodeFlags_Selected : 0) |
                            (in.children().empty() ? ImGuiTreeNodeFlags_Leaf : 0) |
                            ImGuiTreeNodeFlags_OpenOnDoubleClick,
                        "%s", in.name.c_str())) {
    if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemFocused()) && m_selectedMapId != in.id) {
      doMapSelection(in);
    }

    for (auto& mapInfo : in.children()) {
      recursiveDrawTree(*mapInfo);
    }
    ImGui::TreePop();
  } else if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemFocused()) && m_selectedMapId != in.id &&
             in.id != 0) {
    doMapSelection(in);
  } else if ((ImGui::IsMouseReleased(ImGuiMouseButton_Right) && m_selectedMapId != in.id)) {
    ImGui::OpenPopup("maps_popup");
  }
  if (ImGui::BeginPopup("maps_popup", ImGuiWindowFlags_Popup)) {
    if (ImGui::Button("New...", ImVec2(200.0f, 0.0f))) {
      // TODO: Add new map to directory based on current location. Maybe add it as a subdirectory of the current parent?
    }

    ImVec2 cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    if (ImGui::Button("Load...", ImVec2(200.0f, 0.0f))) {
      // TODO: Inserts a map from templates?
    }

    cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    ImGui::Separator();

    cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    if (ImGui::Button("Copy", ImVec2(200.0f, 0.0f))) {
      // TODO: Map -> Clipboard
    }

    cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    if (ImGui::Button("Paste", ImVec2(200.0f, 0.0f))) {
      // TODO: Clipboard -> Map inserted
    }

    cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    if (ImGui::Button("Delete", ImVec2(200.0f, 0.0f))) {
      // TODO: Deletes a map
    }

    cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    ImGui::Separator();

    cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    if (ImGui::Button("Find...", ImVec2(200.0f, 0.0f))) {
      // TODO: Finds a map based on a string input
    }

    cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    if (ImGui::Button("Find Next", ImVec2(200.0f, 0.0f))) {
      // TODO: FInds the next map result based on a string input
    }

    cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    if (ImGui::Button("Find Previous", ImVec2(200.0f, 0.0f))) {
      // TODO: FInds the previous map result based on a string input
    }
    cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    ImGui::Separator();

    cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    if (ImGui::Button("Shift...", ImVec2(200.0f, 0.0f))) {
      // TODO: Moves the map position in directory
    }

    cursorPos = ImGui::GetCursorPos();
    // Move back up a couple couple pixels
    cursorPos.y -= 4.f;
    ImGui::SetCursorPos(cursorPos);

    if (ImGui::Button("Generate Dungeon...", ImVec2(200.0f, 0.0f))) {
      // ???
    }
    ImGui::EndPopup();
  }

  if (ImGui::IsItemToggledOpen()) {
    in.expanded ^= 1;
  }
  ImGui::PopID();
}

void Project::drawMapTree() {
  if (ImGui::Begin("Maps")) {
    if (!m_mapInfos.empty()) {
      recursiveDrawTree(m_mapInfos.root());
    }
  }
  ImGui::End();
}

inline int roundUp(int numToRound, int multiple) {
  if (multiple == 0)
    return numToRound;

  int remainder = numToRound % multiple;
  if (remainder == 0)
    return numToRound;

  return numToRound + multiple - remainder;
}

inline int alignCoord(int value, int size) { return roundUp(value - (value % size), size); }

void Project::drawMapEditor() {
  static float timer = 0.5f;
  if (timer <= 0.f) {
    timer = 0.5f;
  }
  timer -= (1 / 60.f);

  // Keep mapScale to a quarter step

  if (m_map) {
    //    m_mapScale = std::min((m_map->width * 48) / m_initialScrollX, (m_map->height * 48) / m_initialScrollY);
    if (m_initialScrollSet) {
      ImGui::SetScrollX(m_initialScrollX);
      ImGui::SetScrollY(m_initialScrollY);
      m_initialScrollX = m_initialScrollY = 0.0;
      m_initialScrollSet = false;
    }
    m_mapInfos.map(m_selectedMapId)->scrollX = ImGui::GetScrollX();
    m_mapInfos.map(m_selectedMapId)->scrollY = ImGui::GetScrollY();
  }

  if (ImGui::IsMouseKey(ImGuiKey_MouseWheelY) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
    m_mapScale += ImGui::GetIO().MouseWheel * 0.25f;
  }
  m_mapScale = (floorf((m_mapScale * 2.f) + .25f) / 2.f);
  m_mapScale = std::clamp(m_mapScale, .25f, 4.f);

  if (ImGui::Begin("Map Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar)) {
    ImGui::BeginChild("##mapcontents", ImVec2(0, ImGui::GetContentRegionMax().y - 70.f), ImGuiChildFlags_Border,
                      ImGuiWindowFlags_HorizontalScrollbar);
    if (m_map) {
      auto events = m_map->getSortedBy();
      // m_mapRenderer.update();

      ImGuiWindow* win = ImGui::GetCurrentWindow();
      Texture dummyTex = m_resourceManager->loadParallaxImage("Map509");
      ImGui::Dummy(ImVec2{m_map->width * (48 * m_mapScale), m_map->height * (48 * m_mapScale)});

      win->DrawList->AddImage(dummyTex.get(), win->ContentRegionRect.Min,
                              win->ContentRegionRect.Min +
                                  ImVec2{(m_map->width * 48) * m_mapScale, (m_map->height * 48) * m_mapScale});

      for (int y = 0; y <= (m_map->height * 48) * m_mapScale; y += 48 * m_mapScale) {
        win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{0.f, static_cast<float>(y)},
                               win->ContentRegionRect.Min +
                                   ImVec2{(m_map->width * 48) * m_mapScale, static_cast<float>(y)},
                               0x7f0a0a0a, 3.f);
      }

      for (int x = 0; x <= (m_map->width * 48) * m_mapScale; x += 48 * m_mapScale) {
        win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{static_cast<float>(x), 0.f},
                               win->ContentRegionRect.Min +
                                   ImVec2{static_cast<float>(x), (m_map->height * 48) * m_mapScale},
                               0x7f0a0a0a, 3.f);
      }
      for (auto& event : events) {
        if (event) {
          auto eventX = static_cast<float>(event->x * 48) * m_mapScale;
          auto eventY = static_cast<float>(event->y * 48) * m_mapScale;
          eventX += win->ContentRegionRect.Min.x;
          eventY += win->ContentRegionRect.Min.y;
          float eventS = 48 * m_mapScale;
          ImVec2 evMin = ImVec2{eventX, eventY};
          ImVec2 evMax = ImVec2{(eventX + eventS), (eventY + eventS)};
          win->DrawList->AddRectFilled(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f}, 0x7f000000);
          win->DrawList->AddRect(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f}, 0xFF000000, 0, 0, 5.f);
          win->DrawList->AddRect(evMin + ImVec2{1.f, 1.f}, evMax - ImVec2{1.f, 1.f}, 0xFFFFFFFF, 0, 0, 3.f);

          if (!event->pages[0].image.characterName.empty() && event->pages[0].image.tileId == 0) {
            if (event->pages[0].stepAnime) {
              event->pages[0].image.pattern =
                  std::clamp<int>(std::abs(std::remainder(ImGui::GetTime() * 8, 3 * 2)), 0, 3);
            }

            // TODO: This is still wrong
            Texture tex = m_resourceManager->loadCharacterImage(event->pages[0].image.characterName);
            const int CharacterSpriteWidth = tex.width() / 12;
            const int CharacterSpriteHeight = tex.height() / 8;
            const int CharacterAtlasWidth = tex.width() / 4;
            const int CharacterAtlasHeight = tex.height() / 2;

            const float charX = static_cast<float>(
                (event->pages[0].image.characterIndex % (tex.width() / CharacterAtlasWidth)) * CharacterAtlasWidth);
            const float charY = static_cast<float>(
                (event->pages[0].image.characterIndex / (tex.width() / CharacterAtlasWidth)) * CharacterAtlasHeight);
            const float patternOffset = static_cast<float>(event->pages[0].image.pattern * CharacterSpriteWidth);
            const float directionOffset =
                ((static_cast<float>(event->pages[0].image.direction - 2) / 2) * CharacterSpriteHeight);

            float x1 = ((charX + patternOffset) /*+ (CharacterSpriteWidth / 5)*/);
            float y1 = ((charY + directionOffset) /*+ (CharacterSpriteHeight / 4) */);
            float x2 = (((charX + patternOffset) + CharacterSpriteWidth) /*- (CharacterSpriteWidth / 5) */);
            float y2 = (((charY + directionOffset) + CharacterSpriteHeight) /*- (CharacterSpriteHeight / 4) */);

            evMin.x -= ((static_cast<float>(CharacterSpriteWidth) - 48.f) / 2.f) * m_mapScale;
            evMax.x += ((static_cast<float>(CharacterSpriteWidth) - 48.f) / 2.f) * m_mapScale;
            evMin.y -= (static_cast<float>(CharacterSpriteHeight) - 48.f) * m_mapScale;
            win->DrawList->AddImage(
                tex.get(), evMin, evMax,
                ImVec2{x1 / static_cast<float>(tex.width()), y1 / static_cast<float>(tex.height())},
                ImVec2{x2 / static_cast<float>(tex.width()), y2 / static_cast<float>(tex.height())});
          }
        }
      }
      if (ImGui::IsWindowHovered()) {
        ImVec2 cursorPos = ImGui::GetIO().MousePos;

        cursorPos -= win->ContentRegionRect.Min;

        if (!(cursorPos.x < 0 || cursorPos.y < 0 || cursorPos.x >= ((m_map->width * 48) * m_mapScale) ||
              cursorPos.y >= ((m_map->height * 48) * m_mapScale))) {
          if (cursorPos.x < 0) {
            cursorPos.x = 0;
          }
          if (cursorPos.x >= ((m_map->width * 48) * m_mapScale)) {
            cursorPos.x = ((m_map->width * 48) * m_mapScale);
          }

          if (cursorPos.y < 0) {
            cursorPos.y = 0;
          }

          if (cursorPos.y >= ((m_map->height * 48) * m_mapScale)) {
            cursorPos.y = ((m_map->height * 48) * m_mapScale);
          }
          cursorPos.x =
              static_cast<float>(alignCoord(static_cast<int>(cursorPos.x), static_cast<int>(48.f * m_mapScale)));
          cursorPos.y =
              static_cast<float>(alignCoord(static_cast<int>(cursorPos.y), static_cast<int>(48.f * m_mapScale)));

          m_tileCellX = (static_cast<int>(cursorPos.x) + 1) / static_cast<int>(48.f * m_mapScale);
          m_tileCellY = (static_cast<int>(cursorPos.y) + 1) / static_cast<int>(48.f * m_mapScale);
          m_tileId = ((0 * m_map->height) + m_tileCellY) * m_map->width + m_tileCellX;

          cursorPos += win->ContentRegionRect.Min;

          win->DrawList->AddRect(cursorPos + (ImVec2{0.f, 0.f} * m_mapScale), cursorPos + (ImVec2{48, 48} * m_mapScale),
                                 0xFF000000, 0.f, 0, 5.f);
          win->DrawList->AddRect(cursorPos + (ImVec2{0.f, 0.f} * m_mapScale), cursorPos + (ImVec2{48, 48} * m_mapScale),
                                 0xFFFFFFFF, 0.f, 0, 3.f);
        }
      }
      // win->DrawList->AddImage(m_mapRenderer.getUpperBitmap(), win->ContentRegionRect.Min,
      //                         win->ContentRegionRect.Min +
      //                             ImVec2{(m_map->width * 48) * m_mapScale, (m_map->height * 48) * m_mapScale});
    }
    ImGui::EndChild();
    ImGui::Text("Scale:");
    ImGui::SameLine();
    ImGui::SliderFloat("##map_scale", &m_mapScale, 0.25f, 4.f);
    ImGui::SameLine();
    ImGui::Text("Tile %i, (%i, %i)", m_tileId, m_tileCellX, m_tileCellY);
  }
  ImGui::End();
  if (ImGui::Begin("Map Properties")) {
    char buf[4096]{};
    if (m_map) {
      ImGui::BeginGroupPanel();
      {
        ImGui::SeparatorText("General Settings");
        ImGui::BeginGroup();
        {
          ImGui::Text("Name - ID: %.03i", m_selectedMapId);
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
          MapInfo* mapInfo = m_mapInfos.map(m_selectedMapId);
          strncpy(buf, mapInfo->name.c_str(), 4096);
          if (ImGui::InputText("##map_name", buf, 4096)) {
            mapInfo->name = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Display Name");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::InputText("##map_display_name", buf, 4096)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImGui::Text("Tileset");
          strncpy(buf, m_mapInfos.map(m_selectedMapId)->name.c_str(), 4096);
          std::string text = m_tilesets.tileset(m_map->tilesetId)->name.empty()
                                 ? "##map_tileset_button_empty"
                                 : m_tilesets.tileset(m_map->tilesetId)->name;
          ImGui::PushID("##map_tileset_button");
          if (ImGui::Button(text.c_str(), ImVec2{ImGui::GetWindowContentRegionMax().x / 2 - 15, 0})) {}
          ImGui::PopID();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Width");
          ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - 15);
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::DragInt("##map_width", &m_map->width, 0, 0, 256)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Height");
          ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - 15);
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::DragInt("##map_height", &m_map->height, 0, 0, 256)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Scroll Type");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::BeginCombo("##map_scroll_type",
                                DecodeEnumName(magic_enum::enum_name(m_map->scrollType)).c_str())) {
            for (const auto& e : magic_enum::enum_values<ScrollType>()) {
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(e)).c_str(), e == m_map->scrollType)) {
                m_map->scrollType = e;
              }
              if (e == m_map->scrollType) {
                ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Enc. Steps");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::DragInt("##map_enc_steps", &m_map->encounterStep, 0, 0, 999)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
      }
      ImGui::Separator();
      {
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Autoplay BGM", &m_map->autoPlayBgm)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->autoPlayBgm);
          {
            ImGui::PushID("##map_bgm_button");
            ImGui::SetNextItemWidth((ImGui::GetWindowContentRegionMax().x / 2) - 30);
            std::string text = m_map->bgm.name.empty() ? "##map_bgm_button_empty" : m_map->bgm.name;
            if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetWindowContentRegionMax().x / 2) - 15, 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Autoplay BGS", &m_map->autoPlayBgs)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->autoPlayBgs);
          {
            ImGui::PushID("##map_bgs_button");
            std::string text = m_map->bgs.name.empty() ? "##map_bgs_button_empty" : m_map->bgs.name;
            if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetWindowContentRegionMax().x / 2) - 15, 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Battleback", &m_map->specifyBattleBack)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->specifyBattleBack);
          {
            ImGui::PushID("##map_battleback_button");
            // TODO: Combine battleBack1Name and battleBack2Name
            std::string text = m_map->bgs.name.empty() ? "##map_battleback_button_empty" : m_map->battleBack1Name;
            if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetWindowContentRegionMax().x / 2) - 15, 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Disable Dashing", &m_map->disableDashing)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->disableDashing);
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
      }
      ImGui::Separator();
      {
        ImGui::BeginGroup();
        {
          ImGui::Text("Parallax Background");
          ImGui::PushID("##map_parallax_button");
          std::string text = m_map->bgs.name.empty() ? "##map_parallax_button_empty" : m_map->parallaxName;
          if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetWindowContentRegionMax().x / 2) - 15, 0})) {}
          ImGui::PopID();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Loop Horizontally", &m_map->parallaxLoopX)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->parallaxLoopX);
          {
            ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
            if (ImGui::DragInt("##map_parallax_Sx", &m_map->parallaxSx, 0, 0, 999)) {}
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Loop Vertically", &m_map->parallaxLoopY)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->parallaxLoopY);
          {
            ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
            if (ImGui::DragInt("##map_parallax_Sy", &m_map->parallaxSy, 0, 0, 999)) {}
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Show in Editor", &m_map->parallaxShow)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->parallaxShow);
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          ImGuiInputTextFlags flags = ImGuiInputTextFlags_CtrlEnterForNewLine;
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Note");

          strncpy(buf, m_map->note.c_str(), 4096);
          if (ImGui::InputTextMultiline("##map_note", buf, 2048, ImVec2(ImGui::GetContentRegionMax().x - 15, 400),
                                        flags)) {
            m_map->note = buf;
          }
        }
        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::EndGroup();
      }
      ImGui::EndGroupPanel();
    }
  }
  ImGui::End();

  if (m_map) {
    const Tileset* tileset = m_tilesets.tileset(m_map->tilesetId);
    if (tileset && !tileset->name.empty() && !tileset->flags.empty()) {
      if (ImGui::Begin("Tile Debugger")) {
        ImGui::BeginGroup();
        {
          for (int z = 0; z < 6; ++z) {
            int tileCellIndex = (z * m_map->height + m_tileCellY) * m_map->width + m_tileCellX;
            if (tileCellIndex > m_map->data.size()) {
              continue;
            }
            ImGui::Text("Tile %i, layer %i, cell {%i, %i}, absolute {%i, %i}", tileCellIndex, z, m_tileCellX,
                        m_tileCellY, m_tileCellX * 48, m_tileCellY * 48);
            int tileId = m_map->data[tileCellIndex];
            if (tileId >= tileset->flags.size()) {
              continue;
            }
            int flags = tileset->flags[tileId];
            ImGui::Text(
                "Tile ID %i, Flags 0x%04X, IsAutoTile %i, IsA1Tile %i, IsA2Tile %i, IsA3Tile %i, IsA4Tile %i, IsA5Tile "
                "%i",
                tileId, flags, MapRenderer::isAutoTile(tileId), MapRenderer::isTileA1(tileId),
                MapRenderer::isTileA2(tileId), MapRenderer::isTileA3(tileId), MapRenderer::isTileA4(tileId),
                MapRenderer::isTileA5(tileId));
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          for (int z = 0; z < 6; ++z) {
            int tileCellIndex = (z * m_map->height + m_tileCellY) * m_map->width + m_tileCellX;
            if (tileCellIndex < m_map->data.size()) {
              int tileId = m_map->data[tileCellIndex];
              if (MapRenderer::isAutoTile(tileId)) {
                continue;
              }

              int setNumber = 0;
              if (MapRenderer::isTileA5(tileId)) {
                setNumber = 4;
              } else {
                setNumber = 5 + floor(tileId / 256);
              }

              int w = 48;
              int h = 48;
              int sx = (static_cast<int>(floor(tileId / 128)) % 2 * 8 + tileId % 8) * 2;
              int sy = (static_cast<int>(floor(tileId % 128 / 8)) % 16) * h;
              if (tileset->tilesetNames[setNumber].empty()) {
                continue;
              }

              Texture tex = m_resourceManager->loadTilesetImage(tileset->tilesetNames[setNumber]);
              if (tex) {
                ImGui::Image(tex.get(), ImVec2{48, 48},
                             {static_cast<float>(sx) / tex.width(), static_cast<float>(sy) / tex.height()},
                             {static_cast<float>(sx + w) / tex.width(), static_cast<float>(sy + h) / tex.height()});
              }
            }
          }
        }
        ImGui::EndGroup();
      }
    }
    ImGui::End();
  }
}
