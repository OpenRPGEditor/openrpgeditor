#include "Dialog_ChangeTileset.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ChangeTileset::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{300, 95} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    if (tileset_picker) {
      auto [closed, confirmed] = tileset_picker->draw();
      if (confirmed) {
        m_tileset = tileset_picker->selection();
        tileset_picker.reset();
      }
    }

    ImGui::SeparatorText("Tileset");
    ImGui::PushID("##tileset_selection");
    if (ImGui::Button((std::format("{:04}", m_tileset) + Database::Instance->tilesetName(m_tileset)).c_str(),
                      ImVec2{(App::DPIHandler::scale_value(280)), 0})) {
      tileset_picker = ObjectPicker<Tileset>("Tileset"sv, Database::Instance->tilesets.tilesets(), 0);
    }
    ImGui::PopID();

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->tileset = m_tileset;
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
