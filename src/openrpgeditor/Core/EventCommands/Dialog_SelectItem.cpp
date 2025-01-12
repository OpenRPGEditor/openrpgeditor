#include "Core/EventCommands/Dialog_SelectItem.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_SelectItem::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{200, 140}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (closed) {
        if (confirmed) {
          m_variable = picker->selection();
        }
        picker.reset();
      }
    }
    ImGui::SeparatorText("Variable");
    ImGui::SetNextItemWidth(100);
    ImGui::PushID("##inputnumber_variable");
    if (ImGui::Button(Database::instance()->variableNameOrId(m_variable).c_str(), ImVec2{180, 0})) {
      picker.emplace("Variables", Database::instance()->system.variables(), m_variable);
      picker->setOpen(true);
    }
    ImGui::PopID();
    ImGui::SeparatorText("Item Type");
    ImGui::PushItemWidth(180);
    if (ImGui::BeginCombo("##itemtype_selection", DecodeEnumName(magic_enum::enum_value<ItemType>(m_type)).c_str())) {
      for (auto& itemType : magic_enum::enum_values<ItemType>()) {
        bool is_selected = m_type == magic_enum::enum_index(itemType).value();
        if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(itemType)).c_str(), is_selected)) {
          m_type = magic_enum::enum_index(itemType).value();
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->item = m_variable;
      command->type = static_cast<ItemType>(m_type);
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
