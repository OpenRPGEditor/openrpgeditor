#include "Core/CommonUI/TilesetPicker.hpp"
#include "Core/Project.hpp"

#include "imgui.h"

void TilesetPicker::draw() {
  Map* map = m_parent->currentMap();
  if (ImGui::Begin("Tilesets")) {
    if (ImGui::BeginTabBar("##tileset")) {
      if (ImGui::BeginTabItem("  A  ", nullptr)) {
        if (map) {
          Texture tilesetTxtr =
              ResourceManager::instance()->loadTilesetImage(m_parent->tileset(map->tilesetId)->tilesetNames[0]);
          if (tilesetTxtr) {
            ImGui::Image(tilesetTxtr.get(),
                         ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          }
        }
        ImGui::EndTabItem();
      }
      if (map && !m_parent->tileset(map->tilesetId)->tilesetNames[5].empty()) {
        if (ImGui::BeginTabItem("  B  ", nullptr)) {
          Texture tilesetTxtr =
              ResourceManager::instance()->loadTilesetImage(m_parent->tileset(map->tilesetId)->tilesetNames[5]);
          if (tilesetTxtr) {
            ImGui::Image(tilesetTxtr.get(),
                         ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          }
          ImGui::EndTabItem();
        }
      }
      if (map && !m_parent->tileset(map->tilesetId)->tilesetNames[6].empty()) {
        if (ImGui::BeginTabItem("  C  ", nullptr)) {
          Texture tilesetTxtr =
              ResourceManager::instance()->loadTilesetImage(m_parent->tileset(map->tilesetId)->tilesetNames[6]);
          if (tilesetTxtr) {
            ImGui::Image(tilesetTxtr.get(),
                         ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          }
          ImGui::EndTabItem();
        }
      }
      if (map && !m_parent->tileset(map->tilesetId)->tilesetNames[7].empty()) {
        if (ImGui::BeginTabItem("  D  ", nullptr)) {
          Texture tilesetTxtr =
              ResourceManager::instance()->loadTilesetImage(m_parent->tileset(map->tilesetId)->tilesetNames[7]);
          if (tilesetTxtr) {
            ImGui::Image(tilesetTxtr.get(),
                         ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          }
          ImGui::EndTabItem();
        }
      }
      // TODO: This needs to be either generated or made from scratch.
      if (ImGui::BeginTabItem("  R  ", nullptr)) {
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
  }
  ImGui::End();
}