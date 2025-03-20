#include "Core/EventCommands/Dialog_InputNumber.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_InputNumber::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
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
    ImGui::PushID("##inputnumber_variable");
    if (ImGui::Button(Database::instance()->variableNameOrId(m_variable).c_str(), ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
      picker.emplace("Variables", Database::instance()->system.variables(), m_variable);
      picker->setOpen(true);
    }
    ImGui::PopID();
    ImGui::SeparatorText("Digits");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
    if (ImGui::InputInt("##inputnumber_digits", &m_digits)) {
      if (m_digits > 9) {
        m_digits = 9;
      } else if (m_digits < 1)
        m_digits = 1;
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->variable = m_variable;
      command->digits = m_digits;
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
