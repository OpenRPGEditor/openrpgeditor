#include "Editor/CommonUI/EventTilePicker.hpp"

#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "GroupBox.hpp"

#include "Database/Database.hpp"
#include "orei18n.hpp"

#include <Database/MapInfos.hpp>
#include <imgui.h>
#include <imgui_internal.h>
EventTilePicker::EventTilePicker(const int mapId, const bool showTree)
: IDialogController(trNOOP("Select Location"))
, m_currentMap(mapId)
, m_showTree(showTree) {

  for (const auto& mapInfo : Database::instance()->mapInfos->mapInfos()) {
    if (!mapInfo) {
      continue;
    }

    m_expandedState[mapInfo->id()] = mapInfo->expanded();
  }
}

std::tuple<bool, bool> EventTilePicker::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###EventTilePicker");
  }

  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 56) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 16) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###EventTilePicker", tr(m_name)).c_str())) {
    ImGui::BeginVertical("##event_tile_picker_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      ImGui::BeginHorizontal("##event_tile_picker_main_layout", {-1, 0});
      {
        if (m_showTree) {
          ImGui::BeginChild("##event_tile_picker_map_tree", {ImGui::GetContentRegionAvail().x * .30f, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()},
                            ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders, ImGuiWindowFlags_NoBackground);
          { drawMapTreeRecursive(Database::instance()->mapInfos->root()); }
          ImGui::EndChild();
        }
        ImGui::BeginChild("##event_tile_picker_map_canvas", {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()}, ImGuiChildFlags_Borders,
                          ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
        {
          // TODO: Replace all of this with the tilemap canvas when that is rewritten
          const auto map = Database::instance()->mapInfos->map(m_currentMap);
          std::string nameStr = map ? map->id() == 0 ? Database::instance()->system->gameTitle() : map->name() : "No Valid Map Selected";

          if (nameStr.empty()) {
            if (map) {
              if (map->id() != 0) {
                nameStr = std::format("Map{:03}", map->id());
              } else {
                nameStr = map->name();
              }
            } else {
              nameStr = std::format("No Valid Map Selected");
            }
          }
          ImGui::TextUnformatted("PLACEHOLDER DIALOG!");
          ImGui::Text("Map: %s\n", nameStr.c_str());
          GroupBox position("Position", "##event_tile_picker_position", {0, 0});
          if (position.begin()) {
            ImGui::BeginVertical("##event_tile_picker_position", {-1, 0});
            {
              ImGui::BeginHorizontal("##event_tile_picker_x", {-1, 0});
              {
                GroupBox x("X", "##event_tile_picker_position_y", {0, 0});
                if (x.begin()) {
                  if (ImGui::SpinInt("##event_tile_picker_x", &m_selectedTile.xr(), 1, 1)) {
                    m_selectedTile.xr() = std::clamp(m_selectedTile.xr(), 0, map ? map->map()->width() : 0);
                  }
                }
                x.end();
              }
              ImGui::EndHorizontal();
              ImGui::BeginHorizontal("##event_tile_picker_y", {-1, 0});
              {
                GroupBox y("Y", "##event_tile_picker_position_y", {0, 0});
                if (y.begin()) {
                  if (ImGui::SpinInt("##event_tile_picker_y", &m_selectedTile.yr(), 1, 1)) {
                    m_selectedTile.yr() = std::clamp(m_selectedTile.yr(), 0, map ? map->map()->height() : 0);
                  }
                }
                y.end();
              }
              ImGui::EndHorizontal();
            }
            ImGui::EndVertical();
          }
          position.end();
        }
        ImGui::EndChild();
      }
      ImGui::EndHorizontal();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##event_tile_picker_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##event_tile_picker_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          setOpen(false);
          ImGui::CloseCurrentPopup();
        } else if (ret == 1) {
          m_confirmed = false;
          setOpen(false);
          ImGui::CloseCurrentPopup();
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return {!m_open, m_confirmed};
}

void EventTilePicker::drawMapTreeRecursive(MapInfo& info) {
  const std::string id = std::format("#event_tile_picker_map{:03}_node", info.id());
  std::string nameStr = info.id() == 0 ? Database::instance()->system->gameTitle() : info.name();

  if (nameStr.empty()) {
    nameStr = info.name();
  }

  const bool open = ImGui::TreeNodeEx(id.c_str(),
                                      (m_expandedState[info.id()] ? ImGuiTreeNodeFlags_DefaultOpen : 0) | (info.children().empty() ? ImGuiTreeNodeFlags_Leaf : 0) |
                                          (m_currentMap == info.id() ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick,
                                      "%s", nameStr.c_str());

  if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemFocused() && m_currentMap != info.id()) {
    m_currentMap = info.id();
    info.map(true);
  }

  if (open) {
    for (const auto& mapInfo : info.children()) {
      if (!mapInfo) {
        continue;
      }

      drawMapTreeRecursive(*mapInfo);
    }
  }

  if (ImGui::IsItemToggledOpen()) {
    m_expandedState[info.id()] ^= 1;
  }

  if (open) {
    ImGui::TreePop();
  }
}