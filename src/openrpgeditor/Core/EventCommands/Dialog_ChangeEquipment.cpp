#include "Dialog_ChangeEquipment.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ChangeEquipment::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{181, 185} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    if (actor_picker) {
      auto [closed, confirmed] = actor_picker->draw();
      if (confirmed) {
        m_actor = actor_picker->selection();
        actor_picker.reset();
      }
    }

    ImGui::SeparatorText("Actor");

    ImGui::PushID("##change_equip_actor_selection");
    if (ImGui::Button(
            Database::Instance->actorName(m_actor).c_str(),
            ImVec2{App::DPIHandler::scale_value(160), 0})) {
      actor_picker = ObjectPicker<Actor>("Actor"sv, Database::Instance->actors.actorList(), 0);
            }
    ImGui::PopID();

    ImGui::SeparatorText("Equipment Type");
    ImGui::SetNextItemWidth(160 * App::DPIHandler::get_ui_scale());
    if (ImGui::BeginCombo("##equip_type_list", Database::Instance->equipTypeName(m_equipmentType).c_str())) {
      for (int i = 1; i < Database::Instance->system.equipTypes.size(); ++i) {
        if (ImGui::Selectable(Database::Instance->system.equipTypes.at(i).c_str(), i == m_equipmentType)) {
          m_equipmentType = i;
        }
      }
      ImGui::EndCombo();
    }
    ImGui::SeparatorText("Equipment Item");
    ImGui::SetNextItemWidth(160 * App::DPIHandler::get_ui_scale());
    std::string text = m_equipmentItem == 0 ? "None" :
      (m_equipmentType == 1 ? Database::Instance->weaponName(m_equipmentItem) : Database::Instance->armorName(m_equipmentItem));
    if (ImGui::BeginCombo("##equip_item_list", text.c_str())) {
      if (m_equipmentType == 1) {
        // None
        if (ImGui::Selectable("None", m_equipmentItem == 0)) {
          m_equipmentItem = 0;
        }

        for (auto equips : Database::Instance->weapons.weaponList()) {
          if (equips.etypeId == m_equipmentType && equips.wtypeId != 0) {
            if (ImGui::Selectable(equips.name.c_str(), equips.id == m_equipmentItem)) {
              m_equipmentItem = equips.id;
            }
          }
        }
      }
      else {
        // None
        if (ImGui::Selectable("None", m_equipmentItem == 0)) {
          m_equipmentItem = 0;
        }

        for (auto equips : Database::Instance->armors.armorList()) {
          if (equips.etypeId == m_equipmentType && equips.atypeId != 0) {
            if (ImGui::Selectable(equips.name.c_str(), equips.id == m_equipmentItem)) {
              m_equipmentItem = equips.id;
            }
          }
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->actorId = m_actor;
      command->equipType = m_equipmentType;
      command->equipment = m_equipmentItem;
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
