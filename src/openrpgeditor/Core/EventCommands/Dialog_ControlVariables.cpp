
#include "Dialog_ControlVariables.hpp"
#include "Database/Database.hpp"
#include "Dialog_GameData.hpp"
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"

std::tuple<bool, bool> Dialog_ControlVariables::draw() {

  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{450, 440} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {
    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (confirmed) {
        if (singleRequest) {
          m_variable_var = picker->selection();
        } else {
          m_variable = picker->selection();
        }
        picker.reset();
      }
    }

    if (gameDataDialog) {
      auto [closed, confirmed] = gameDataDialog->draw();
      if (confirmed) {
        m_gameData_type = static_cast<int>(gameDataDialog->getData().gameData.type);
        m_gameData_rawSource = gameDataDialog->getData().gameData.rawSource;
        m_gameData_value = gameDataDialog->getData().gameData.value;
      }
    }

    ImGui::SeparatorText("Variable");
    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Single", &m_operation_var, 0); // Specific Switch
      ImGui::RadioButton("Range", &m_operation_var, 1);  // Range of Switches
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      std::string text = m_operation_var != 0
                             ? "##commonevent_switch_empty"
                             : (m_variable_var == 0 ? "" : std::format("{:04} ", m_variable_var) + Database::Instance->variableName(m_variable_var));
      ImGui::PushID("##controlswitch_id");
      //ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x + 75) - (16 * App::DPIHandler::get_ui_scale()));
      ImGui::BeginDisabled(m_operation_var != 0);
      if (ImGui::Button(
              text.c_str(),
              ImVec2{(ImGui::GetWindowContentRegionMax().x - 100) - 15 * App::DPIHandler::get_ui_scale(), 0})) {
        singleRequest = true;
        picker.emplace("Variables", Database::Instance->system.variables);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::BeginDisabled(m_operation_var != 1);
      ImGui::PushItemWidth(50);
      ImGui::InputInt("##controlswitch_range1", &m_rand_1, 0);
      ImGui::SameLine();
      ImGui::Text("~");
      ImGui::SameLine();
      ImGui::InputInt("##controlswitch_range2", &m_rand_2, 0);
      ImGui::PopItemWidth();
      ImGui::EndDisabled();

      ImGui::EndGroup();
    }
    ImGui::SeparatorText("Operation");
    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Set", &m_operation, 0);
      ImGui::SameLine();
      ImGui::RadioButton("Add", &m_operation, 1);
      ImGui::SameLine();
      ImGui::RadioButton("Sub", &m_operation, 2);
      ImGui::SameLine();
      ImGui::RadioButton("Mul", &m_operation, 3);
      ImGui::SameLine();
      ImGui::RadioButton("Div", &m_operation, 4);
      ImGui::SameLine();
      ImGui::RadioButton("Mod", &m_operation, 5);
      ImGui::EndGroup();
    }

    ImGui::SeparatorText("Operand");
    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Constant", &m_operand, 0);
      ImGui::RadioButton("Variable", &m_operand, 1);
      ImGui::RadioButton("Random", &m_operand, 2);
      ImGui::RadioButton("Game Data", &m_operand, 3);
      ImGui::RadioButton("Script", &m_operand, 4);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {

      // Constant
      ImGui::BeginDisabled(m_operand != 0);
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(75));
      ImGui::InputInt("##controlvariables_constant", &m_constant, 0);
      ImGui::EndDisabled();

      // Variable
      ImGui::BeginDisabled(m_operand != 1);
      std::string text = m_operand != 1 ? "##commonevent_switch_empty"
                                        : std::format("{:04} ", m_variable) + Database::Instance->variableName(m_variable);
      ImGui::PushID("##controlvariable_id2");
      if (ImGui::Button(text.c_str(), ImVec2{App::DPIHandler::scale_value(300), 0})) {
        singleRequest = false;
        picker.emplace("Variables", Database::Instance->system.variables);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      // Random
      ImGui::BeginDisabled(m_operand != 2);
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(75));
      ImGui::InputInt("##controlvariables_range1", &m_rand_operand_1, 0);
      ImGui::SameLine();
      ImGui::Text("~");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(75));
      ImGui::InputInt("##controlvariables_range2", &m_rand_operand_2, 0);
      ImGui::EndDisabled();

      // Game Data
      ImGui::BeginDisabled(m_operand != 3);
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(150));
      text = m_operand != 3 ? "" : getUIString();
      ImGui::PushID("##controlvariable_gamedata");
      if (ImGui::Button(text.c_str(), ImVec2{App::DPIHandler::scale_value(300), 0})) {
        if (!gameDataDialog)
          gameDataDialog.emplace("Game Data", m_gameData_type, m_gameData_rawSource, m_gameData_value);

        gameDataDialog->SetOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      // Script
      ImGui::BeginDisabled(m_operand != 4);
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(300));
      if (ImGui::InputText("##controlvariables_script", &m_script)) {
        command->script = m_script;
      }
      ImGui::EndDisabled();
      ImGui::EndGroup();
    }

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      if (m_operation_var == 0) {
        command->start = m_variable_var;
        command->end = m_variable_var;
      } else {
        command->start = m_rand_1;
        command->end = m_rand_2;
      }
      command->operation = static_cast<VariableControlOperation>(m_operation);
      command->operand = static_cast<VariableControlOperand>(m_operand);
      if (command->operand == VariableControlOperand::Constant) {
        command->constant = m_constant;
      } else if (command->operand == VariableControlOperand::Variable) {
        command->variable = m_variable;
      } else if (command->operand == VariableControlOperand::Random) {
        command->random.min = m_rand_operand_1;
        command->random.max = m_rand_operand_2;
      } else if (command->operand == VariableControlOperand::Script) {
        command->script = m_script;
      } else if (command->operand == VariableControlOperand::Game_Data) {
        command->gameData.type = static_cast<GameDataType>(m_gameData_type);
        command->gameData.rawSource = m_gameData_rawSource;
        if (command->gameData.type == GameDataType::Actor
            || command->gameData.type == GameDataType::Enemy
            || command->gameData.type == GameDataType::Character) {
          command->gameData.value = m_gameData_value;
        }
      }
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
