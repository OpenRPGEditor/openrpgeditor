#include "Core/EventCommands/Dialog_ShakeScreen.hpp"

#include "Core/DPIHandler.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ShakeScreen::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{410, 190} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Shake");
    ImGui::BeginGroup();
    {
      ImGui::Text("Power:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Speed:");
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(150));
      ImGui::SliderInt("##shakescreen_power", &m_power, -1, 9, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(75));
      if (ImGui::InputInt("##shakescreen_power_int", &m_power, 1, 100)) {
        if (m_power < 1)
          m_power = 1;
        if (m_power > 9)
          m_power = 9;
      }

      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(150));
      ImGui::SliderInt("##shakescreen_speed", &m_speed, 1, 9, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(75));
      if (ImGui::InputInt("##shakescreen_speed_int", &m_speed, 1, 100)) {
        if (m_speed < 1)
          m_speed = 1;
        if (m_speed > 9)
          m_speed = 9;
      }
    }
    ImGui::EndGroup();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.f);
    ImGui::PushItemWidth((App::DPIHandler::scale_value(390)));
    if (ImGui::BeginCombo("##flashscreen_presets", "Future presets go in this list")) {
      // TODO: Template system
      ImGui::EndCombo();
    }
    ImGui::SeparatorText("Duration");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
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
      command->power = m_power;
      command->speed = m_speed;
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
