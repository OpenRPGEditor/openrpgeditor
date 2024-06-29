#include "Core/MapListView.hpp"
#include "Core/Project.hpp"
#include "Database/MapInfos.hpp"

#include "imgui.h"

void MapListView::recursiveDrawTree(MapInfo& in) {
  std::string id = ("#orpg_map_" + std::to_string(in.id) + in.name);
  bool open = ImGui::TreeNodeEx(id.c_str(),
                                (in.expanded ? ImGuiTreeNodeFlags_DefaultOpen : 0) |
                                    (in.children().empty() ? ImGuiTreeNodeFlags_Leaf : 0) |
                                    (m_selectedMapId == in.id ? ImGuiTreeNodeFlags_Selected : 0) |
                                    ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick,
                                "%s", in.name.c_str());
  if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemFocused()) && m_selectedMapId != in.id) {
    m_parent->setMap(in);
  }

  if (ImGui::BeginDragDropSource()) {
    if (in.id != 0) {
      auto payload = m_mapInfos->map(in.id);
      ImGui::SetDragDropPayload("##orpg_dnd_mapinfo", payload, sizeof(payload));
      ImGui::Text("%s", payload->name.c_str());
    }
    ImGui::EndDragDropSource();
  }

  if (ImGui::BeginDragDropTarget()) {
    const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("##orpg_dnd_mapinfo");
    if (payload) {
      MapInfo* src = static_cast<MapInfo*>(payload->Data);
      if (src) {
        m_mapInfos->map(src->id)->parentId = in.id;
        m_mapTreeStale = true;
      }
    }
    ImGui::EndDragDropTarget();
  }
  if (ImGui::BeginPopupContextWindow()) {
    // Ensure we have the right clicked map selected
    if (m_selectedMapId != in.id && ImGui::IsItemHovered()) {
      m_parent->setMap(in);
    }
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
  if (open) {
    for (auto& mapInfo : in.children()) {
      recursiveDrawTree(*mapInfo);
    }
  }

  if (ImGui::IsItemToggledOpen()) {
    in.expanded ^= 1;
  }
  if (open) {
    ImGui::TreePop();
  }
}

void MapListView::draw() {
  if (ImGui::Begin("Maps", nullptr, ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
    if (m_mapInfos && !m_mapInfos->empty()) {
      recursiveDrawTree(m_mapInfos->root());
    }
  }
  ImGui::End();

  if (m_mapInfos && m_mapTreeStale) {
    m_mapInfos->buildTree(true);
    m_mapTreeStale = false;
  }
}

MapInfo* MapListView::currentMapInfo() { return m_mapInfos->map(m_selectedMapId); }
const MapInfo* MapListView::currentMapInfo() const { return m_mapInfos->map(m_selectedMapId); }