
#include "Dialog_ControlSwitches.hpp"
#include "imgui.h"

void Dialog_ControlSwitches::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name);
    SetOpen(false);
  }

  if (ImGui::BeginPopupModal(m_name, &m_open, ImGuiWindowFlags_NoResize)) {
    ImGui::Text("Stacked Modal");
    if (ImGui::Button("Close")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}
