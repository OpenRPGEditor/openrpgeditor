
#include "Dialog_ControlVariables.hpp"
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"
#include "Core/Project.hpp"

void Dialog_ControlVariables::draw() {

  if (IsOpen()) {
    ImGui::OpenPopup(m_name);
    //SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{350, 340} * App::DPIHandler::get_scale());
  if (ImGui::BeginPopupModal(m_name, &m_open, ImGuiWindowFlags_NoScrollbar)) {
    if (picker) {
      auto [closed, confirmed]  = picker->draw();
      if (confirmed) {
        if (singleRequest) {
          command.start = picker->selection();
          command.end = picker->selection();
        }
        else {
          command.variable = picker->selection();
        }
        picker.reset();
      }
    }
    ImGui::SeparatorText("Switch");
    static int switchType = 0;
    ImGui::RadioButton("Single", &switchType, 0); // Specific Switch
    ImGui::SameLine();

    bool isSwitchEnabled = switchType == 1;
    std::string text =
        isSwitchEnabled ? "##commonevent_switch_empty" : std::format("{:04} ", command.start) + m_project->variable(command.start);
    ImGui::PushID("##controlswitch_id");
    ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - (16 * App::DPIHandler::get_scale()));
    ImGui::BeginDisabled(isSwitchEnabled);
    static int variableId = 0;
    if (ImGui::Button(text.c_str(), ImVec2{((ImGui::GetWindowContentRegionMax().x / 2) / 2) -
                                               (15 * App::DPIHandler::get_scale()),
                                           0})) {
      singleRequest = true;
      picker.emplace("Variables", m_project->system().variables);
    }
    ImGui::PopID();
    ImGui::EndDisabled();

    bool isRangeEnabled = switchType == 0;
    static int rand1 = 0;
    static int rand2 = 1;

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
    static int operation = 0;
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
    static int operand = 0;
    ImGui::RadioButton("Constant", &operand, 0);
    ImGui::SameLine();
    ImGui::BeginDisabled(operand != 0);
    ImGui::PushItemWidth(100);
    ImGui::InputInt("##controlvariables_constant", &command.constant, 0);
    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    // Variable
    ImGui::RadioButton("Variable", &operand, 1);
    ImGui::SameLine();
    ImGui::BeginDisabled(operand != 1);
    ImGui::PushItemWidth(100);
    text =
        operand != 1 ? "##commonevent_switch_empty" : ""; //m_parent->switches(m_selectedCommonEvent->switchId);
    ImGui::PushID("##controlvariable_id");
    if (ImGui::Button(text.c_str(), ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) -
                                               (15 * App::DPIHandler::get_scale()),
                                           0})) {
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
    ImGui::InputInt("##controlvariables_range1", &command.random.min, 0);
    ImGui::SameLine();
    ImGui::Text("~");
    ImGui::SameLine();
    ImGui::InputInt("##controlvariables_range2", &command.random.max, 0);
    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    // Game Data
    ImGui::RadioButton("Game Data", &operand, 3);
    ImGui::SameLine();
    ImGui::BeginDisabled(operand != 3);
    ImGui::PushItemWidth(100);
    text =
        operand != 3 ? "##commonevent_switch_empty" : ""; //m_parent->switches(m_selectedCommonEvent->switchId);
    ImGui::PushID("##controlvariable_gamedata");
    if (ImGui::Button(text.c_str(), ImVec2{((ImGui::GetWindowContentRegionMax().x / 2)) -
                                               (15 * App::DPIHandler::get_scale()),
                                           0})) {
      // Open Menu to select switch
      // TODO
    }
    ImGui::PopID();
    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    // Script
    ImGui::RadioButton("Script", &operand, 4);
    ImGui::SameLine();
    ImGui::BeginDisabled(operand != 4);
    ImGui::PushItemWidth(100);
    char script[4096];
    if (ImGui::InputText("##controlvariables_script", script, IM_ARRAYSIZE(script))) {
      command.script = script;
    }
    ImGui::PopItemWidth();
    ImGui::EndDisabled();


    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 75, ImGui::GetCursorPosY()));
    if (ImGui::Button("OK")) {
      if (isSwitchEnabled) {
        command.start = variableId;
        command.end = variableId;
      }
      else {
        command.start = rand1;
        command.end = rand2;
      }
      command.operation = static_cast<VariableControlOperation>(operation);
      command.operand = static_cast<VariableControlOperand>(operand);

      // Insert command into m_commands?
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      APP_DEBUG(std::to_string(ImGui::GetContentRegionMax().x) +  ", " + std::to_string(ImGui::GetContentRegionMax().y));
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::EndPopup();
  }
}
