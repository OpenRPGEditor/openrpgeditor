#include "Core/MapListView.hpp"
#include "Core/Project.hpp"
#include "Database/MapInfos.hpp"

#include "imgui.h"

void MapListView::recursiveDrawTree(MapInfo& in) {
  const std::string id = ("#orpg_map_" + std::to_string(in.id) + in.name);
  bool open = ImGui::TreeNodeEx(id.c_str(),
                                (in.expanded ? ImGuiTreeNodeFlags_DefaultOpen : 0) | (in.children().empty() ? ImGuiTreeNodeFlags_Leaf : 0) |
                                    (m_selectedMapId == in.id ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick,
                                "%s", in.name.c_str());
  if (m_selectedMapId == in.id && m_needsScroll) {
    ImGui::SetScrollHereY();
    m_needsScroll = false;
  }

  if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemFocused()) && m_selectedMapId != in.id) {
    m_selectedMapId = in.id;
    m_parent->setMap(*m_mapInfos->map(in.id));
  }

  if (ImGui::BeginDragDropSource()) {
    if (in.id != 0) {
      auto payload = m_mapInfos->map(in.id);
      ImGui::SetDragDropPayload("##orpg_dnd_mapinfo", payload, sizeof(uintptr_t));
      ImGui::Text("%s", payload->name.c_str());
    }
    ImGui::EndDragDropSource();
  }

  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("##orpg_dnd_mapinfo")) {
      if (const auto* src = static_cast<MapInfo*>(payload->Data)) {
        // Are we trying to put our parent into ourselves?
        if (in.parentId != src->id) {
          // Nope, we can just safely set the parent id without worry
          m_mapInfos->map(src->id)->parentId = in.id;
        } else {
          // Yes, we have to swap parents, and assign the new parent
          m_mapInfos->map(in.id)->parentId = m_mapInfos->map(src->id)->parentId;
          m_mapInfos->map(src->id)->parentId = in.id;
        }
        /* Set ordering to force it at the end */
        if (!in.children().empty()) {
          m_mapInfos->map(src->id)->order = in.children().back()->order + 1;
        }
        m_mapTreeStale = true;
      }
    }
    ImGui::EndDragDropTarget();
  }
  if (ImGui::BeginPopupContextWindow()) {
    // Ensure we have the right-clicked map selected
    if (m_selectedMapId != in.id && ImGui::IsItemHovered()) {
      m_selectedMapId = in.id;
      m_parent->setMap(in);
    }
    if (ImGui::MenuItem("New...")) {
      // TODO: Add new map to directory based on current location. Maybe add it as a subdirectory of the current parent?
    }

    if (ImGui::Selectable("Load...")) {
      // TODO: Inserts a map from templates?
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Copy")) {
      // TODO: Map -> Clipboard
    }

    if (ImGui::MenuItem("Paste")) {
      // TODO: Clipboard -> Map inserted
    }

    if (ImGui::MenuItem("Delete")) {
      // TODO: Deletes a map
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Find...")) {
      // TODO: Finds a map based on a string input
    }

    if (ImGui::MenuItem("Find Next")) {
      // TODO: FInds the next map result based on a string input
    }

    if (ImGui::MenuItem("Find Previous")) {
      // TODO: FInds the previous map result based on a string input
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Shift...")) {
      // TODO: Moves the map position in directory
    }

    if (ImGui::MenuItem("Generate Dungeon...")) {
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
#define PRINT_MAP_ORDERING 0
#if PRINT_MAP_ORDERING
static bool orderingPrinted = false;
void recursivePrintOrdering(const MapInfo& in) {
  static int indent = 0;
  std::cout << std::string(indent, '\t') + std::format("id {} name {} order {}", in.id, in.name, in.order) << std::endl;
  indent++;
  for (auto& mapInfo : in.children()) {
    recursivePrintOrdering(*mapInfo);
  }
  indent--;
}
#endif

void MapListView::draw() {
  if (ImGui::Begin("Maps", nullptr, ImGuiWindowFlags_HorizontalScrollbar)) {
    if (m_mapInfos && !m_mapInfos->empty()) {
      recursiveDrawTree(m_mapInfos->root());
    }
  }
  ImGui::End();

  if (m_mapInfos && m_mapTreeStale) {
    m_mapInfos->buildTree(true);
    m_mapInfos->rebuildOrdering();
    m_mapTreeStale = false;
#if PRINT_MAP_ORDERING
    orderingPrinted = false;
#endif
  }

#if PRINT_MAP_ORDERING
  if (m_mapInfos && !orderingPrinted) {
    recursivePrintOrdering(m_mapInfos->root());
    orderingPrinted = true;
  }
#endif
}

void MapListView::recursiveExpandParents(MapInfo& in) {
  if (!in.expanded && m_selectedMapId != in.id) {
    in.expanded = true;
  }

  if (in.parentId != 0) {
    recursiveExpandParents(*m_mapInfos->map(in.parentId));
  }
}

void MapListView::setCurrentMapId(const int id, const bool doExpand) {
  if (id == m_selectedMapId) {
    return;
  }

  m_selectedMapId = id;

  if (doExpand && m_selectedMapId >= 0) {
    recursiveExpandParents(*m_mapInfos->map(m_selectedMapId));
  }
  m_needsScroll = true;
}

MapInfo* MapListView::currentMapInfo() { return m_mapInfos->map(m_selectedMapId); }
const MapInfo* MapListView::currentMapInfo() const { return m_mapInfos->map(m_selectedMapId); }