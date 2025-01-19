#include "Core/EventCommands/Dialog_FlashScreen.hpp"

#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_FlashScreen::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{410, 250}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Flash Color");

    color_picker.draw();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.f);
    ImGui::PushItemWidth(390);
    if (ImGui::BeginCombo("##flashscreen_presets", "Future presets go in this list")) {
      // TODO: Template system for tint screen
      ImGui::EndCombo();
    }
    ImGui::SeparatorText("Duration");
    ImGui::SetNextItemWidth(100);
    if (ImGui::InputInt("##flashscreen_duration", &m_duration)) {
      if (m_duration < 1)
        m_duration = 1;
      if (m_duration > 999)
        m_duration = 999;
    }
    ImGui::SameLine();
    ImGui::Text("frames 1/60 sec");
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 80.f);
    ImGui::Checkbox("Wait for Completion", &m_waitForCompletion);

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->color.r = color_picker.r();
      command->color.g = color_picker.g();
      command->color.b = color_picker.b();
      command->color.intensity = color_picker.intensity();
      command->duration = m_duration;
      command->waitForCompletion = m_waitForCompletion;
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
