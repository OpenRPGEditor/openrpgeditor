#include "Core/EventCommands/Dialog_ChangeGold.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeGold::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{402, 160}, ImGuiCond_Appearing);
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

    ImGui::SeparatorText("Operation");
    ImGui::RadioButton("Increase", &m_operation, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Decrease", &m_operation, 1);
    ImGui::SeparatorText("Operand");

    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Constant", &m_operandSource, 0);
      ImGui::RadioButton("Variable", &m_operandSource, 1);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_operandSource != 0);
      ImGui::SetNextItemWidth(100);
      ImGui::InputInt("##changegold_constant_input", &m_constant, 1, 100);
      ImGui::EndDisabled();

      ImGui::BeginDisabled(m_operandSource != 1);
      ImGui::SetNextItemWidth(100);
      std::string text = m_operandSource != 1 ? "##commonevent_switch_empty" : Database::instance()->variableNameOrId(m_variable);
      ImGui::PushID("##controlvariable_gamedata");
      if (ImGui::Button(text.c_str(), ImVec2{200 - 15, 0})) {
        picker.emplace("Variables", Database::instance()->system.variables, m_variable);
        picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();
      ImGui::EndGroup();
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->operation = static_cast<QuantityChangeOp>(m_operation);
      command->operandSource = static_cast<QuantityChangeSource>(m_operandSource);
      if (command->operandSource == QuantityChangeSource::Constant) {
        command->operand = m_constant;
      } else {
        command->operand = m_variable;
      }
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
