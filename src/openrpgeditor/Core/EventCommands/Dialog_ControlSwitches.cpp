
#include "Dialog_ControlSwitches.hpp"
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"
#include "Core/Project.hpp"

std::tuple<bool, bool> Dialog_ControlSwitches::draw() {

  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    //SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{300, 200} * App::DPIHandler::get_ui_scale());
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
    if (picker) {
      auto [closed, confirmed]  = picker->draw();
      if (confirmed) {
        n_start = picker->selection();
        m_end = picker->selection();
        picker.reset();
      }
    }

    ImGui::SeparatorText("Switch");
    static int switchType = 0;
    ImGui::PushItemWidth(200);
    ImGui::RadioButton("Single", &switchType, 0); // Specific Switch
    ImGui::PopItemWidth();
    ImGui::SameLine();

    bool isSwitchEnabled = switchType == 1;
    std::string text =
        isSwitchEnabled ? "##commonevent_switch_empty" : (n_start == 0 ? "" : std::format("{:04} ", n_start) + m_project->switche(command->start));
    ImGui::PushID("##controlswitch_id");
    ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x + 50) - (16 * App::DPIHandler::get_ui_scale()));
    ImGui::BeginDisabled(isSwitchEnabled);
    if (ImGui::Button(text.c_str(), ImVec2{((ImGui::GetWindowContentRegionMax().x - 75)) -
                                               (15 * App::DPIHandler::get_ui_scale()),
                                           0})) {
      picker.emplace("Switches", m_project->system().switches);
    }
    ImGui::PopID();
    ImGui::EndDisabled();

    bool isRangeEnabled = switchType == 0;

    ImGui::RadioButton("Range", &switchType, 1); // Range of Switches
    ImGui::SameLine();
    ImGui::BeginDisabled(isRangeEnabled);
    ImGui::PushItemWidth(50);
    ImGui::InputInt("##controlswitch_range1", &m_rand_1, 0);
    ImGui::SameLine();
    ImGui::Text("~");
    ImGui::SameLine();
    ImGui::InputInt("##controlswitch_range2", &m_rand_2, 0);
    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    ImGui::SeparatorText("Operation");
    static int operationBool = 0;
    ImGui::RadioButton("ON", &operationBool, 0); // Specific Switch
    ImGui::SameLine();
    ImGui::RadioButton("OFF", &operationBool, 1); // Range of Switches

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 75, ImGui::GetCursorPosY()));
    if (ImGui::Button("OK")) {
      if (isSwitchEnabled) {
        command->start = n_start;
        command->end = m_end;
      }
      else {
        command->start = m_rand_1;
        command->end = m_rand_2;
      }
      command->turnOff = static_cast<ValueControl>(operationBool);

      // Insert command into m_commands?
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      SetOpen(false);
    }
    ImGui::EndPopup();
  }
  return std::make_tuple(!m_open, m_confirmed);
}
