#include "Core/EventCommands/Dialog_ChangeParameter.hpp"


#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeParameter::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{254, 270}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (m_actorPicker) {
      auto [closed, confirmed] = m_actorPicker->draw();
      if (closed) {
        if (confirmed) {
          m_value = m_actorPicker->selection();
        }
        m_actorPicker.reset();
      }
    }
    if (m_picker) {
      auto [closed, confirmed] = m_picker->draw();
      if (closed) {
        if (confirmed) {
          if (m_isOperand)
            m_quantity_var = m_picker->selection();
          else
            m_value_var = m_picker->selection();
        }
        m_picker.reset();
      }
    }
    // Section 1 (Actor: Fixed/Variable)
    ImGui::SeparatorText("Actor");
    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Fixed", &m_comparison, 0);
      ImGui::RadioButton("Variable", &m_comparison, 1);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_comparison != 0);
      ImGui::PushID("##changemp_actor");
      if (ImGui::Button(m_comparison == 0 ? Database::instance()->actorNameAndId(m_value).c_str() : "", ImVec2{200 - 15, 0})) {
        m_actorPicker = ObjectPicker<Actor>("Actor"sv, Database::instance()->actors.actorList(), m_value);
        m_actorPicker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::BeginDisabled(m_comparison != 1);
      ImGui::PushID("##changemp_var");
      if (ImGui::Button(m_comparison == 1 ? Database::instance()->variableNameAndId(m_value_var).c_str() : "", ImVec2{200 - 15, 0})) {
        m_isOperand = false;
        m_picker.emplace("Variables", Database::instance()->system.variables, m_value_var);
        m_picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::EndGroup();
    }
    // Section 3 Parameters
    ImGui::SeparatorText("Parameter");
    ImGui::PushItemWidth((160));
    if (ImGui::BeginCombo("##parameter_selection", DecodeEnumName(magic_enum::enum_value<ParameterSource>(m_parameterSource)).c_str())) {
      for (auto& parameter : magic_enum::enum_values<ParameterSource>()) {
        bool is_selected = m_parameterSource == magic_enum::enum_index(parameter).value();
        if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(parameter)).c_str(), is_selected)) {
          m_parameterSource = magic_enum::enum_index(parameter).value();
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    // Section 3 (Operation: Increase/Decrease)
    ImGui::SeparatorText("Operation");
    ImGui::RadioButton("Increase", &m_quantityOp, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Decrease", &m_quantityOp, 1);

    // Section 4 (Operand: Constant/Variable)
    ImGui::SeparatorText("Operand");
    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Constant", &m_quantitySource, 0);
      ImGui::RadioButton("Variable##2", &m_quantitySource, 1);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_quantitySource != 0);
      ImGui::SetNextItemWidth(100);
      if (ImGui::InputInt("##changemp_constant", &m_quantity)) {
        if (m_quantity > 9999)
          m_quantity = 9999;
        if (m_quantity < 0)
          m_quantity = 0;
      }
      ImGui::EndDisabled();

      ImGui::BeginDisabled(m_quantitySource != 1);
      ImGui::PushID("##changemp_quant_var");
      if (ImGui::Button(m_quantitySource == 1 ? Database::instance()->variableNameAndId(m_quantity_var).c_str() : "", ImVec2{200 - 15, 0})) {
        m_isOperand = true;
        m_picker.emplace("Variables", Database::instance()->system.variables, m_quantity_var);
        m_picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::EndGroup();
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;

      m_command->comparison = static_cast<ActorComparisonSource>(m_comparison);
      m_command->quantityOp = static_cast<QuantityChangeOp>(m_quantityOp);
      m_command->quantitySource = static_cast<QuantityChangeSource>(m_quantitySource);
      m_command->param = static_cast<ParameterSource>(m_parameterSource);

      if (m_command->comparison == ActorComparisonSource::Variable)
        m_command->value = m_value_var;
      else
        m_command->value = m_value;

      if (m_command->quantitySource == QuantityChangeSource::Variable)
        m_command->quantity = m_quantity_var;
      else
        m_command->quantity = m_quantity;

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
