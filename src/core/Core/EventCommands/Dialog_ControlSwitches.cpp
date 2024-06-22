
#include "Dialog_ControlSwitches.hpp"
#include "imgui.h"
#include "Core/DPIHandler.hpp"

void Dialog_ControlSwitches::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name);
    //SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{300, 250} * App::DPIHandler::get_scale());
  if (ImGui::BeginPopupModal(m_name, &m_open, ImGuiWindowFlags_NoResize)) {
    ImGui::SeparatorText("Switch");
    static int switchType = 0;
    ImGui::RadioButton("Single", &switchType, 0); // Specific Switch
    ImGui::SameLine();

    bool isSwitchEnabled = switchType == 1;
    std::string text =
        isSwitchEnabled ? "##commonevent_switch_empty" : ""; //m_parent->switches(m_selectedCommonEvent->switchId);
    ImGui::PushID("##controlswitch_id");
    ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) / 2 - (16 * App::DPIHandler::get_scale()));
    ImGui::BeginDisabled(isSwitchEnabled);
    if (ImGui::Button(text.c_str(), ImVec2{((ImGui::GetWindowContentRegionMax().x / 2) / 2) -
                                               (15 * App::DPIHandler::get_scale()),
                                           0})) {
      // Open Menu to select switch
                                           }
    ImGui::PopID();
    ImGui::EndDisabled();

    bool isRangeEnabled = switchType == 0;

    ImGui::RadioButton("Range", &switchType, 1); // Range of Switches
    static int rand1;
    static int rand2;

    ImGui::BeginDisabled(isRangeEnabled);
    ImGui::PushItemWidth(100);
    if (ImGui::InputInt("##controlswitch_range1", &rand1, 0)) {
      // m_rand1 = rand1;
    }
    ImGui::SameLine();
    ImGui::Text("~");
    ImGui::SameLine();
    if (ImGui::InputInt("##controlswitch_range2", &rand2, 0)) {
      // m_rand2 = rand2;
    }
    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    ImGui::SeparatorText("Operation");
    static int operationBool = 0;
    ImGui::RadioButton("ON", &operationBool, 1); // Specific Switch
    ImGui::SameLine();
    ImGui::RadioButton("OFF", &operationBool, 0); // Range of Switches

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 75, ImGui::GetCursorPosY()));
    if (ImGui::Button("OK")) {
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
