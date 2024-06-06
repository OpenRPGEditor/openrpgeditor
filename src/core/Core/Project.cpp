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
  // Load the previously loaded map
  doMapSelection(m_mapInfos.m_mapinfos[m_system.editMapId]);
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
              m_resourceManager->loadTilesetImage(m_tilesets.tileset(m_map->tilesetId)->tilesetNames[0]);
          ImGui::Image(tilesetTxtr.get(),
                       ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
        }
        ImGui::EndTabItem();
      }
      if (m_map && !m_tilesets.tileset(m_map->tilesetId)->tilesetNames[5].empty()) {
        if (ImGui::BeginTabItem("B", nullptr)) {
          Texture tilesetTxtr =
              m_resourceManager->loadTilesetImage(m_tilesets.tileset(m_map->tilesetId)->tilesetNames[5]);
          ImGui::Image(tilesetTxtr.get(),
                       ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          ImGui::EndTabItem();
        }
      }
      if (m_map && !m_tilesets.tileset(m_map->tilesetId)->tilesetNames[6].empty()) {
        if (ImGui::BeginTabItem("C", nullptr)) {
          Texture tilesetTxtr =
              m_resourceManager->loadTilesetImage(m_tilesets.tileset(m_map->tilesetId)->tilesetNames[6]);
          ImGui::Image(tilesetTxtr.get(),
                       ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          ImGui::EndTabItem();
        }
      }
      if (m_map && !m_tilesets.tileset(m_map->tilesetId)->tilesetNames[7].empty()) {
        if (ImGui::BeginTabItem("D", nullptr)) {
          Texture tilesetTxtr =
              m_resourceManager->loadTilesetImage(m_tilesets.tileset(m_map->tilesetId)->tilesetNames[7]);
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
  m_mapRenderer.setMap(&*m_map, m_tilesets.tileset(m_map->tilesetId));
  m_initialScrollX = m_mapInfos.m_mapinfos[m_selectedMapId].scrollX;
  m_initialScrollY = m_mapInfos.m_mapinfos[m_selectedMapId].scrollX;

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
    if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemFocused()) && m_selectedMapId != in.id) {
      doMapSelection(in);
    }

    for (auto& mapInfo : in.m_children) {
      recursiveDrawTree(*mapInfo);
    }
    ImGui::TreePop();
  } else if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemFocused()) && m_selectedMapId != in.id &&
             in.id != 0) {
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
  if (ImGui::Begin("Map Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar)) {
    ImGui::BeginChild("##mapcontents", ImVec2(0, ImGui::GetContentRegionMax().y - 70.f), ImGuiChildFlags_Border,
                      ImGuiWindowFlags_HorizontalScrollbar);
    if (m_map) {
      if (m_initialScrollX != 0.0 || m_initialScrollY != 0.0) {
        m_mapScale = std::min((m_map->width * 48) / m_initialScrollX, (m_map->height * 48) / m_initialScrollY);
        ImGui::SetScrollX(m_initialScrollX);
        ImGui::SetScrollY(m_initialScrollY);
        m_initialScrollX = m_initialScrollY = 0.0;
      }

      if (ImGui::IsMouseKey(ImGuiKey_MouseWheelY) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
        m_mapScale += ImGui::GetIO().MouseWheel * 0.5f;
        m_mapScale = std::clamp(m_mapScale, 0.5f, 4.f);
      }

      // HACK: For testing, either change to a random texture, add a Map509 to parallaxes, or bug
      // Phil to implement tilemap rendering lmao
      Texture tilesetTxtr = m_resourceManager->loadParallaxImage("Map509");
      ImGui::Image(tilesetTxtr.get(), ImVec2{(m_map->width * 48) * m_mapScale, (m_map->height * 48) * m_mapScale});
      ImGuiWindow* win = ImGui::GetCurrentWindow();

      for (int y = 0; y <= (m_map->height * 48) * m_mapScale; y += 48 * m_mapScale) {
        win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{0.f, static_cast<float>(y)},
                               win->ContentRegionRect.Min +
                                   ImVec2{(m_map->width * 48) * m_mapScale, static_cast<float>(y)},
                               0x7f0a0a0a, 1.f);
      }

      for (int x = 0; x <= (m_map->width * 48) * m_mapScale; x += 48 * m_mapScale) {
        win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{static_cast<float>(x), 0.f},
                               win->ContentRegionRect.Min +
                                   ImVec2{static_cast<float>(x), (m_map->height * 48) * m_mapScale},
                               0x7f0a0a0a, 1.f);
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

          float thickness = 4.f - m_mapScale;
          if (thickness < 2.f) {
            thickness = 2.f;
          }
          win->DrawList->AddRect(cursorPos + (ImVec2{1.f, 1.f} * m_mapScale), cursorPos + (ImVec2{47, 47} * m_mapScale),
                                 0xFF000000, 0.f, 0, thickness);
          win->DrawList->AddRect(cursorPos + (ImVec2{-1.f, -1.f} * m_mapScale),
                                 cursorPos + (ImVec2{49, 49} * m_mapScale), 0xFF000000, 0.f, 0, thickness);
          win->DrawList->AddRect(cursorPos + (ImVec2{0.f, 0.f} * m_mapScale), cursorPos + (ImVec2{48, 48} * m_mapScale),
                                 0xFFFFFFFF, 0.f, 0, thickness);
        }
      }
    }
    ImGui::EndChild();
    ImGui::Text("Scale:");
    ImGui::SameLine();
    ImGui::DragFloat("##map_scale", &m_mapScale, 0.25, 0.5, 4.0);
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
          strncpy(buf, m_mapInfos.m_mapinfos[m_selectedMapId].name.c_str(), 4096);
          if (ImGui::InputText("##map_name", buf, 4096)) {
            m_mapInfos.m_mapinfos[m_selectedMapId].name = buf;
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
          strncpy(buf, m_mapInfos.m_mapinfos[m_selectedMapId].name.c_str(), 4096);
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
      }
      ImGui::EndGroupPanel();
    }
  }
  ImGui::End();

  if (m_map) {
    const Tileset* tileset = m_tilesets.tileset(m_map->tilesetId);
    if (ImGui::Begin("Tile Debugger")) {
      ImGui::BeginGroup();
      {
        for (int z = 0; z < 6; ++z) {
          int tileCellIndex = (z * m_map->height + m_tileCellY) * m_map->width + m_tileCellX;
          if (tileCellIndex > m_map->data.size()) {
            continue;
          }
          ImGui::Text("Tile %i, layer %i, cell {%i, %i}, absolute {%i, %i}", tileCellIndex, z, m_tileCellX, m_tileCellY,
                      m_tileCellX * 48, m_tileCellY * 48);
          int tileId = m_map->data[tileCellIndex];
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

            if (tileId >= 0 && tileId < MapRenderer::TILE_ID_C && !tileset->tilesetNames[5].empty()) {
              Texture bTexture = m_resourceManager->loadTilesetImage(tileset->tilesetNames[5]);
              int tileX = (tileId / (bTexture.width() / 48)) * 48;
              int tileY = (tileId % (bTexture.width() / 48)) * 48;

              ImGui::Image(bTexture.get(), ImVec2{48, 48},
                           ImVec2{static_cast<float>(tileX), static_cast<float>(tileY)} / bTexture.width(),
                           ImVec2{(static_cast<float>(tileX) + 48.f), (static_cast<float>(tileY) + 48.f)} /
                               bTexture.height());
            }

            if (tileId >= MapRenderer::TILE_ID_C && tileId < MapRenderer::TILE_ID_D &&
                !tileset->tilesetNames[6].empty()) {
              int tileIndex = tileId - MapRenderer::TILE_ID_C;
              Texture cTexture = m_resourceManager->loadTilesetImage(tileset->tilesetNames[6]);
              int tileX = (tileIndex / (cTexture.width() / 48)) * 48;
              int tileY = (tileIndex % (cTexture.width() / 48)) * 48;

              ImGui::Image(cTexture.get(), ImVec2{48, 48},
                           ImVec2{static_cast<float>(tileX), static_cast<float>(tileY)} / cTexture.width(),
                           ImVec2{(static_cast<float>(tileX) + 48.f), (static_cast<float>(tileY) + 48.f)} /
                               cTexture.height());
            }

            if (tileId >= MapRenderer::TILE_ID_D && tileId < MapRenderer::TILE_ID_E &&
                !tileset->tilesetNames[7].empty()) {
              int tileIndex = tileId - MapRenderer::TILE_ID_D;
              Texture dTexture = m_resourceManager->loadTilesetImage(tileset->tilesetNames[7]);
              int tileX = (tileIndex / (dTexture.width() / 48)) * 48;
              int tileY = (tileIndex % (dTexture.width() / 48)) * 48;

              ImGui::Image(dTexture.get(), ImVec2{48, 48},
                           ImVec2{static_cast<float>(tileX), static_cast<float>(tileY)} / dTexture.width(),
                           ImVec2{(static_cast<float>(tileX) + 48.f), (static_cast<float>(tileY) + 48.f)} /
                               dTexture.height());
            }

            if (tileId >= MapRenderer::TILE_ID_E && tileId < MapRenderer::TILE_ID_A1 &&
                !tileset->tilesetNames[8].empty()) {
              int tileIndex = tileId - MapRenderer::TILE_ID_E;
              Texture eTexture = m_resourceManager->loadTilesetImage(tileset->tilesetNames[7]);
              int tileX = (tileIndex / (eTexture.width() / 48)) * 48;
              int tileY = (tileIndex % (eTexture.width() / 48)) * 48;

              ImGui::Image(eTexture.get(), ImVec2{48, 48},
                           ImVec2{static_cast<float>(tileX), static_cast<float>(tileY)} / eTexture.width(),
                           ImVec2{(static_cast<float>(tileX) + 48.f), (static_cast<float>(tileY) + 48.f)} /
                               eTexture.height());
            }

            if (MapRenderer::isTileA1(tileId) && !tileset->tilesetNames[0].empty()) {
              int tileIndex = tileId - MapRenderer::TILE_ID_A1;
              Texture a1Texture = m_resourceManager->loadTilesetImage(tileset->tilesetNames[0]);
              int tileX = (tileIndex / (a1Texture.width() / 48)) * 48;
              int tileY = (tileIndex % (a1Texture.width() / 48)) * 48;

              ImGui::Image(a1Texture.get(), ImVec2{48, 48},
                           ImVec2{static_cast<float>(tileX), static_cast<float>(tileY)} / a1Texture.width(),
                           ImVec2{(static_cast<float>(tileX) + 48.f), (static_cast<float>(tileY) + 48.f)} /
                               a1Texture.height());
            }

            if (MapRenderer::isTileA2(tileId) && !tileset->tilesetNames[1].empty()) {
              int tileIndex = tileId - MapRenderer::TILE_ID_A2;
              Texture a2Texture = m_resourceManager->loadTilesetImage(tileset->tilesetNames[1]);
              assert(a2Texture.width() > 0);
              int tileX = (tileIndex / (a2Texture.width() / 48)) * 48;
              int tileY = (tileIndex % (a2Texture.width() / 48)) * 48;

              ImGui::Image(a2Texture.get(), ImVec2{48, 48},
                           ImVec2{static_cast<float>(tileX), static_cast<float>(tileY)} / a2Texture.width(),
                           ImVec2{(static_cast<float>(tileX) + 48.f), (static_cast<float>(tileY) + 48.f)} /
                               a2Texture.height());
            }

            if (MapRenderer::isTileA3(tileId) && !tileset->tilesetNames[2].empty()) {
              int tileIndex = tileId - MapRenderer::TILE_ID_A3;
              Texture a3Texture = m_resourceManager->loadTilesetImage(tileset->tilesetNames[2]);
              int tileX = (tileIndex / (a3Texture.width() / 48)) * 48;
              int tileY = (tileIndex % (a3Texture.width() / 48)) * 48;

              ImGui::Image(a3Texture.get(), ImVec2{48, 48},
                           ImVec2{static_cast<float>(tileX), static_cast<float>(tileY)} / a3Texture.width(),
                           ImVec2{(static_cast<float>(tileX) + 48.f), (static_cast<float>(tileY) + 48.f)} /
                               a3Texture.height());
            }
          }
        }
      }
      ImGui::EndGroup();
    }
    ImGui::End();
  }
}