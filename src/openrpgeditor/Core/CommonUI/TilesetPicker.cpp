#include "Core/CommonUI/TilesetPicker.hpp"
#include "Core/Project.hpp"

#include "imgui.h"

void TilesetPicker::draw() {
  const Map* map = m_parent->currentMap();
  if (ImGui::Begin("Tilesets")) {
    if (ImGui::BeginTabBar("##tileset")) {
      if (ImGui::BeginTabItem("  A  ", nullptr)) {
        if (map) {
          if (const Texture tilesetTxtr = ResourceManager::instance()->loadTilesetImage(m_parent->tileset(map->tilesetId())->tilesetName(0))) {
            ImGui::Image(tilesetTxtr, ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          }
        }
        ImGui::EndTabItem();
      }
      if (map && !m_parent->tileset(map->tilesetId())->tilesetName(5).empty()) {
        if (ImGui::BeginTabItem("  B  ", nullptr)) {
          if (const Texture tilesetTxtr = ResourceManager::instance()->loadTilesetImage(m_parent->tileset(map->tilesetId())->tilesetName(5))) {
            ImGui::Image(tilesetTxtr, ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          }
          ImGui::EndTabItem();
        }
      }
      if (map && !m_parent->tileset(map->tilesetId())->tilesetName(6).empty()) {
        if (ImGui::BeginTabItem("  C  ", nullptr)) {
          if (const Texture tilesetTxtr = ResourceManager::instance()->loadTilesetImage(m_parent->tileset(map->tilesetId())->tilesetName(6))) {
            ImGui::Image(tilesetTxtr, ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
          }
          ImGui::EndTabItem();
        }
      }
      if (map && !m_parent->tileset(map->tilesetId())->tilesetName(7).empty()) {
        if (ImGui::BeginTabItem("  D  ", nullptr)) {
          if (const Texture tilesetTxtr = ResourceManager::instance()->loadTilesetImage(m_parent->tileset(map->tilesetId())->tilesetName(7))) {
            ImGui::Image(tilesetTxtr, ImVec2{static_cast<float>(tilesetTxtr.width()), static_cast<float>(tilesetTxtr.width())});
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