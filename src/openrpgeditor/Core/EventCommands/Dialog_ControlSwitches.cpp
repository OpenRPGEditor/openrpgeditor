
#include "Core/EventCommands/Dialog_ControlSwitches.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"
#include "Database/Database.hpp"
#include "imgui.h"

std::tuple<bool, bool> Dialog_ControlSwitches::draw() {

  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{300, 200} * App::DPIHandler::get_ui_scale());
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (closed) {
        if (confirmed) {
          m_start = picker->selection();
          m_end = picker->selection();
        }
        picker.reset();
      }
    }

    ImGui::SeparatorText("Switch");
    ImGui::RadioButton("Single", &m_operation, 0); // Specific Switch
    ImGui::SameLine();

    std::string text = m_operation != 0 ? "##commonevent_switch_empty" : (m_start == 0 ? "" : std::format("{:04} ", m_start) + Database::instance()->switchName(m_start));
    ImGui::PushID("##controlswitch_id");
    ImGui::BeginDisabled(m_operation != 0);
    if (ImGui::Button(text.c_str(), ImVec2{(App::DPIHandler::scale_value(160)), 0})) {
      picker.emplace("Switches", Database::instance()->system.switches, m_start);
      picker->setOpen(true);
    }
    ImGui::PopID();
    ImGui::EndDisabled();

    ImGui::RadioButton("Range", &m_operation, 1); // Range of Switches
    ImGui::SameLine();
    ImGui::BeginDisabled(m_operation != 1);
    ImGui::PushItemWidth(App::DPIHandler::scale_value(50));
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

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - 78, ImGui::GetCursorPosY()));
    if (ImGui::Button("OK")) {
      m_confirmed = true;
      if (m_operation == 0) {
        command->start = m_start;
        command->end = m_end;
      } else {
        command->start = m_rand_1;
        command->end = m_rand_2;
      }
      command->turnOff = static_cast<ValueControl>(operationBool);
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      setOpen(false);
    }
    ImGui::EndPopup();
  }
  return std::make_tuple(!m_open, m_confirmed);
}
