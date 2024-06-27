
#include "Dialog_ControlVariables.hpp"

#include "Dialog_GameData.hpp"
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"
#include "Core/Project.hpp"

void Dialog_ControlVariables::draw() {

  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{450, 440} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar)) {
    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (confirmed) {
        if (singleRequest) {
          d_start = picker->selection();
          d_end = picker->selection();
        } else {
          d_variable = picker->selection();
        }
        picker.reset();
      }
    }

    if (gameDataDialog)
      gameDataDialog->draw();

    ImGui::SeparatorText("Variable");
    static int switchType = 0;
    ImGui::RadioButton("Single", &switchType, 0); // Specific Switch
    ImGui::SameLine();

    bool isSwitchEnabled = switchType == 1;
    std::string text = isSwitchEnabled
                           ? "##commonevent_switch_empty"
                           : (d_start == 0 ? "" : std::format("{:04} ", d_start) + m_project->variable(d_start));
    ImGui::PushID("##controlswitch_id");
    ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x + 75) - (16 * App::DPIHandler::get_ui_scale()));
    ImGui::BeginDisabled(isSwitchEnabled);
    static int variableId = 0;
    if (ImGui::Button(text.c_str(),
                      ImVec2{(ImGui::GetWindowContentRegionMax().x - 100) - 15 * App::DPIHandler::get_ui_scale(), 0})) {
      singleRequest = true;
      picker.emplace("Variables", m_project->system().variables);
    }
    ImGui::PopID();
    ImGui::EndDisabled();

    bool isRangeEnabled = switchType == 0;
    static int rand1 = 0;
    static int rand2 = 0;
    static int operation = 0;
    static int operand = 0;

    ImGui::RadioButton("Range", &switchType, 1); // Range of Switches
    ImGui::SameLine();

    ImGui::BeginDisabled(isRangeEnabled);
    ImGui::PushItemWidth(50);
    ImGui::InputInt("##controlswitch_range1", &rand1, 0);
    ImGui::SameLine();
    ImGui::Text("~");
    ImGui::SameLine();
    ImGui::InputInt("##controlswitch_range2", &rand2, 0);
    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    ImGui::SeparatorText("Operation");
    ImGui::RadioButton("Set", &operation, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Add", &operation, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Sub", &operation, 2);
    ImGui::SameLine();
    ImGui::RadioButton("Mul", &operation, 3);
    ImGui::SameLine();
    ImGui::RadioButton("Div", &operation, 4);
    ImGui::SameLine();
    ImGui::RadioButton("Mod", &operation, 5);

    // Constant
    ImGui::SeparatorText("Operand");
    ImGui::RadioButton("Constant", &operand, 0);
    ImGui::SameLine();
    ImGui::BeginDisabled(operand != 0);
    ImGui::PushItemWidth(100);
    ImGui::InputInt("##controlvariables_constant", &d_constant, 0);
    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    // Variable
    ImGui::RadioButton("Variable", &operand, 1);
    ImGui::SameLine();
    ImGui::BeginDisabled(operand != 1);
    ImGui::PushItemWidth(100);
    text = operand != 1 ? "##commonevent_switch_empty"
                        : (d_variable == 0 ? "" : std::format("{:04} ", d_variable) + m_project->variable(d_variable));
    ImGui::PushID("##controlvariable_id2");
    if (ImGui::Button(text.c_str(),
                      ImVec2{(ImGui::GetWindowContentRegionMax().x - 75) - 15 * App::DPIHandler::get_ui_scale(), 0})) {
      singleRequest = false;
      picker.emplace("Variables", m_project->system().variables);
    }
    ImGui::PopID();
    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    // Random
    ImGui::RadioButton("Random", &operand, 2);
    ImGui::SameLine();
    ImGui::BeginDisabled(operand != 2);
    ImGui::PushItemWidth(100);
    ImGui::InputInt("##controlvariables_range1", &d_rand_1, 0);
    ImGui::SameLine();
    ImGui::Text("~");
    ImGui::SameLine();
    ImGui::InputInt("##controlvariables_range2", &d_rand_2, 0);
    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    // Game Data
    ImGui::RadioButton("Game Data", &operand, 3);
    ImGui::SameLine();
    ImGui::BeginDisabled(operand != 3);
    ImGui::PushItemWidth(100);
    text = operand != 3 ? "##commonevent_switch_empty" : ""; // m_parent->switches(m_selectedCommonEvent->switchId);
    ImGui::PushID("##controlvariable_gamedata");
    if (ImGui::Button(
            text.c_str(),
            ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) - (15 * App::DPIHandler::get_ui_scale()), 0})) {

      gameDataDialog = new Dialog_GameData("Game Data", m_project);
      gameDataDialog->SetOpen(true);
    }
    ImGui::PopID();
    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    // Script
    ImGui::RadioButton("Script", &operand, 4);
    ImGui::SameLine();
    ImGui::BeginDisabled(operand != 4);
    ImGui::PushItemWidth(100);
    if (ImGui::InputText("##controlvariables_script", &script)) {
      command->script = script;
    }
    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    ImGui::SetCursorPos(
        ImVec2(ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(100) - ImGui::GetStyle().FramePadding.x,
               ImGui::GetCursorPosY()));
    if (ImGui::Button("OK")) {
      if (isSwitchEnabled) {
        command->start = variableId;
        command->end = variableId;
      } else {
        command->start = rand1;
        command->end = rand2;
      }
      command->operation = static_cast<VariableControlOperation>(operation);
      command->operand = static_cast<VariableControlOperand>(operand);
      command->constant = d_constant;
      command->variable = d_variable;
      command->random.min = d_rand_1;
      command->random.max = d_rand_2;
      // Insert command into m_commands?
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
}
