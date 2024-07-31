#include "Dialog_ChangeTP.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ChangeTP::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{254, 227} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (actor_picker) {
      auto [closed, confirmed] = actor_picker->draw();
      if (confirmed) {
        m_value = actor_picker->selection();
        actor_picker.reset();
      }
    }
    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (confirmed) {
        if (isOperand)
          m_quantity_var = picker->selection();
        else
          m_value_var  = picker->selection();

        picker.reset();
      }
    }
    // Section 1 (Actor: Fixed/Variable)
    ImGui::SeparatorText("Actor");
    ImGui::BeginGroup(); {
      ImGui::RadioButton("Fixed", &m_comparison, 0);
      ImGui::RadioButton("Variable", &m_comparison, 1);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup(); {
      ImGui::BeginDisabled(m_comparison != 0);
      ImGui::PushID("##changetp_actor");
      if (ImGui::Button(
              m_comparison == 0 ? (std::format("{:04} ", m_value) + Database::Instance->actorName(m_value)).c_str() : "",
              {(App::DPIHandler::scale_value(160)), 0})) {

        actor_picker = ObjectPicker<Actor>("Actor"sv, Database::Instance->actors.actorList(), 0);
      }
      ImGui::PopID();
      ImGui::EndDisabled();


      ImGui::BeginDisabled(m_comparison != 1);
      ImGui::PushID("##changetp_var");
      if (ImGui::Button(
              m_comparison == 1 ? Database::Instance->variableNameAndId(m_value_var).c_str() : "",
              ImVec2{(App::DPIHandler::scale_value(160)), 0})) {
        isOperand = false;
        picker.emplace("Variables", Database::Instance->system.variables);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::EndGroup();
    }

    // Section 2 (Operation: Increase/Decrease)
    ImGui::SeparatorText("Operation");
    ImGui::RadioButton("Increase", &m_quantityOp, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Decrease", &m_quantityOp, 1);

    // Section 3 (Operand: Constant/Variable)
    ImGui::SeparatorText("Operand");
    ImGui::BeginGroup(); {
      ImGui::RadioButton("Constant", &m_quantitySource, 0);
      ImGui::RadioButton("Variable##2", &m_quantitySource, 1);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup(); {
      ImGui::BeginDisabled(m_quantitySource != 0);
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
      if (ImGui::InputInt("##changetp_constant", &m_quantity)) {
        if (m_quantity > 9999)
          m_quantity = 9999;
        if (m_quantity < 0)
          m_quantity = 0;
      }
      ImGui::EndDisabled();

      ImGui::BeginDisabled(m_quantitySource != 1);
      ImGui::PushID("##changetp_quant_var");
      if (ImGui::Button(
              m_quantitySource == 1 ? Database::Instance->variableNameAndId(m_quantity_var).c_str() : "",
              ImVec2{(App::DPIHandler::scale_value(160)), 0})) {
        isOperand = true;
        picker.emplace("Variables", Database::Instance->system.variables);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::EndGroup();
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;

      command->comparison = static_cast<ActorComparisonSource>(m_comparison);
      command->quantityOp = static_cast<QuantityChangeOp>(m_quantityOp);
      command->quantitySource = static_cast<QuantityChangeSource>(m_quantitySource);

      if (command->comparison == ActorComparisonSource::Variable)
        command->value = m_value_var;
      else
        command->value = m_value;

      if (command->quantitySource == QuantityChangeSource::Variable)
        command->quantity = m_quantity_var;
      else
        command->quantity = m_quantity;

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
