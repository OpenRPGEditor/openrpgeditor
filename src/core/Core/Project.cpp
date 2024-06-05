#include "Project.hpp"

#include "ImGuiFileDialog.h"
#include "ResourceManager.hpp"

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
  std::string version;
  std::ifstream file(filePath.data());
  std::getline(file, version);
  Utils::trim(version);

  auto it = std::find(KnownRPGMVVersions.begin(), KnownRPGMVVersions.end(), version);

  m_isValid = it != KnownRPGMVVersions.end();
  if (!m_isValid) {
    APP_ERROR("Invalid project version {}", version);
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
  m_mapInfos.m_mapinfos[0].expanded = true;
  m_mapInfos.m_mapinfos[0].name = m_system.gameTitle;
  APP_INFO("Loaded project!");
  m_resourceManager.emplace(m_basePath);
  return true;
}

bool Project::close(bool save) {
  if (save) {
    // TODO: Implement when safe to do so
  }

  /* Default initialize all of these */
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
    ImGuiID dock4 = ImGui::DockBuilderSplitNode(dock3, ImGuiDir_Up, 0.175f, nullptr, &dock3);
    // 6. Add windows to each docking space:
    ImGui::DockBuilderDockWindow("Events", dock1);
    ImGui::DockBuilderDockWindow("Tilesets", dock1);
    ImGui::DockBuilderDockWindow("Maps", dock2);
    ImGui::DockBuilderDockWindow("Map Editor", dock3);
    ImGui::DockBuilderDockWindow("Map Properties", dock4);
    ImGui::DockBuilderGetNode(dock3)->SetLocalFlags(ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoTabBar);
    ImGui::DockBuilderGetNode(dock4)->SetLocalFlags(ImGuiDockNodeFlags_HiddenTabBar);

    // 7. We're done setting up our docking configuration:
    ImGui::DockBuilderFinish(mainWindowGroup);
  }
  ImGui::DockSpaceOverViewport(ImGui::GetID("MainWindowGroup"));
}

void Project::draw() {
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

    ImGui::EndMainMenuBar();
  }
}
void Project::drawFileDialog() {
  // First check if we have a pending project request
  if (ImGuiFileDialog::Instance()->Display("OpenProjectDlg", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking,
                                           ImVec2(300, 300))) {
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
              m_resourceManager->loadTilesetImage(m_tilesets.m_tilesets[m_map->tilesetId].tilesetNames[0]);
          ImGui::Image(tilesetTxtr.get(),
                       ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
        }
        ImGui::EndTabItem();
      }
      if (m_map && !m_tilesets.m_tilesets[m_map->tilesetId].tilesetNames[5].empty()) {
        if (ImGui::BeginTabItem("B", nullptr)) {
          Texture tilesetTxtr =
              m_resourceManager->loadTilesetImage(m_tilesets.m_tilesets[m_map->tilesetId].tilesetNames[5]);
          ImGui::Image(tilesetTxtr.get(),
                       ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          ImGui::EndTabItem();
        }
      }
      if (m_map && !m_tilesets.m_tilesets[m_map->tilesetId].tilesetNames[6].empty()) {
        if (ImGui::BeginTabItem("C", nullptr)) {
          Texture tilesetTxtr =
              m_resourceManager->loadTilesetImage(m_tilesets.m_tilesets[m_map->tilesetId].tilesetNames[6]);
          ImGui::Image(tilesetTxtr.get(),
                       ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          ImGui::EndTabItem();
        }
      }
      if (m_map && !m_tilesets.m_tilesets[m_map->tilesetId].tilesetNames[7].empty()) {
        if (ImGui::BeginTabItem("D", nullptr)) {
          Texture tilesetTxtr =
              m_resourceManager->loadTilesetImage(m_tilesets.m_tilesets[m_map->tilesetId].tilesetNames[7]);
          ImGui::Image(tilesetTxtr.get(),
                       ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
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

void Project::doMapSelection(MapInfos::MapInfo& in) {
  if (in.id == 0) {
    m_map.reset();
    m_selectedMapId = 0;
    m_mapRenderer.setMap(nullptr, nullptr);
    return;
  }
  SDL_SetCursor(waitCursor);
  m_selectedMapId = in.id;
  m_map = m_resourceManager->loadMap(in.id);
  SDL_SetCursor(SDL_GetDefaultCursor());
}

void Project::recursiveDrawTree(MapInfos::MapInfo& in) {
  ImGui::PushID(&in);

  if (ImGui::TreeNodeEx(&in,
                        (in.expanded ? ImGuiTreeNodeFlags_DefaultOpen : 0) |
                            (m_selectedMapId == in.id ? ImGuiTreeNodeFlags_Selected : 0) |
                            (in.m_children.empty() ? ImGuiTreeNodeFlags_Leaf : 0) |
                            ImGuiTreeNodeFlags_OpenOnDoubleClick,
                        "%s", in.name.c_str())) {
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && m_selectedMapId != in.id) {
      doMapSelection(in);
    }

    for (auto& mapInfo : in.m_children) {
      recursiveDrawTree(*mapInfo);
    }
    ImGui::TreePop();
  } else if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && m_selectedMapId != in.id && in.id != 0) {
    doMapSelection(in);
  }

  if (ImGui::IsItemToggledOpen()) {
    in.expanded ^= 1;
  }
  ImGui::PopID();
}

void Project::drawMapTree() {
  if (ImGui::Begin("Maps")) {
    if (!m_mapInfos.m_mapinfos.empty()) {
      recursiveDrawTree(m_mapInfos.m_mapinfos[0]);
    }
  }
  ImGui::End();
}

inline int Align(int value, int size) { return (value - (value % size)); }

void Project::drawMapEditor() {
  if (ImGui::Begin("Map Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar)) {
    if (m_map) {
      Texture tilesetTxtr =
          m_resourceManager->loadTilesetImage(m_tilesets.m_tilesets[m_map->tilesetId].tilesetNames[0]);
      ImGui::Image(tilesetTxtr.get(), ImVec2{tilesetTxtr.width() * m_mapScale, tilesetTxtr.height() * m_mapScale});
      ImGuiWindow* win = ImGui::GetCurrentWindow();

      for (int y = 0; y <= tilesetTxtr.height() * m_mapScale; y += 48 * m_mapScale) {
        win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{0.f, static_cast<float>(y)},
                               win->ContentRegionRect.Min +
                                   ImVec2{tilesetTxtr.width() * m_mapScale, static_cast<float>(y)},
                               0x7fa0a0a0, m_mapScale);
      }

      for (int x = 0; x <= tilesetTxtr.width() * m_mapScale; x += 48 * m_mapScale) {
        win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{static_cast<float>(x), 0.f},
                               win->ContentRegionRect.Min +
                                   ImVec2{static_cast<float>(x), tilesetTxtr.height() * m_mapScale},
                               0x7fa0a0a0, m_mapScale);
      }

      ImVec2 cursorPos = ImGui::GetIO().MousePos;

      cursorPos -= win->ContentRegionRect.Min;

      if (!(cursorPos.x < 0 || cursorPos.y < 0 ||
            cursorPos.x > (tilesetTxtr.width() * m_mapScale) + (48 * m_mapScale) ||
            cursorPos.y > (tilesetTxtr.height() * m_mapScale) + (48 * m_mapScale))) {
        if (cursorPos.x < 0) {
          cursorPos.x = 0;
        }
        if (cursorPos.x > (tilesetTxtr.width() * m_mapScale) - (48 * m_mapScale)) {
          cursorPos.x = (tilesetTxtr.width() * m_mapScale) - (48 * m_mapScale);
        }

        if (cursorPos.y < 0) {
          cursorPos.y = 0;
        }

        if (cursorPos.y > (tilesetTxtr.height() * m_mapScale) - (48 * m_mapScale)) {
          cursorPos.y = (tilesetTxtr.height() * m_mapScale) - (48 * m_mapScale);
        }
        cursorPos.x = Align(cursorPos.x, 48 * m_mapScale);
        cursorPos.y = Align(cursorPos.y, 48 * m_mapScale);

        cursorPos += win->ContentRegionRect.Min;

        win->DrawList->AddRect(cursorPos + (ImVec2{0.f, 0.f} * m_mapScale), cursorPos + (ImVec2{48, 48} * m_mapScale),
                               0xFFFFFFFF, 0.f, 0, 2.f * m_mapScale);
        win->DrawList->AddRect(cursorPos + (ImVec2{-2.f, -2.f} * m_mapScale), cursorPos + (ImVec2{50, 50} * m_mapScale),
                               0xFF000000, 0.f, 0, 2.f * m_mapScale);
        win->DrawList->AddRect(cursorPos + (ImVec2{2.f, 2.f} * m_mapScale), cursorPos + (ImVec2{46, 46} * m_mapScale),
                               0xFF000000, 0.f, 0, 2.f * m_mapScale);
      }
    }
  }
  ImGui::End();
  if (ImGui::Begin("Map Properties")) {
    ImGui::DragFloat("Scale", &m_mapScale, 0.25, 0.5, 4.0);
  }
  ImGui::End();
}