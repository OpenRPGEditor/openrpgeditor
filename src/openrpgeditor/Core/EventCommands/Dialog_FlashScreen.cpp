#include "Core/EventCommands/Dialog_FlashScreen.hpp"

#include "Core/DPIHandler.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_FlashScreen::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{410, 250} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Flash Color");
    ImGui::BeginGroup();
    {
      ImGui::Text("Red:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Green:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Blue:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Intensity:");
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(150));
      ImGui::SliderInt("##flashscreen_red", &r, 0, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(75));
      if (ImGui::InputInt("##flashscreen_int_red", &r, 1, 100)) {
        if (r < 0)
          r = 0;
        if (r > 255)
          r = 255;
      }

      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(150));
      ImGui::SliderInt("##flashscreen_green", &g, 0, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(75));
      if (ImGui::InputInt("##flashscreen_int_green", &g, 1, 100)) {
        if (g < 0)
          g = 0;
        if (g > 255)
          g = 255;
      }

      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(150));
      ImGui::SliderInt("##flashscreen_blue", &b, 0, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(75));
      if (ImGui::InputInt("##flashscreen_int_blue", &b, 1, 100)) {
        if (b < 0)
          b = 0;
        if (b > 255)
          b = 255;
      }

      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(150));
      ImGui::SliderInt("##lashscreen_gray", &m_intensity, 0, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(75));
      if (ImGui::InputInt("##flashscreen_int_gray", &m_intensity, 1, 100)) {
        if (m_intensity < 1)
          m_intensity = 1;
        if (m_intensity > 255)
          m_intensity = 255;
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();

    ImGui::ColorButton("##flashscreen_square", ImVec4{static_cast<float>(r * (1.0f / 255.0f)), static_cast<float>(g * (1.0f / 255.0f)), static_cast<float>(b * (1.0f / 255.0f)), 1}, 0,
                       ImVec2{100, 100});

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.f);
    ImGui::PushItemWidth((App::DPIHandler::scale_value(390)));
    if (ImGui::BeginCombo("##flashscreen_presets", "Future presets go in this list")) {
      // TODO: Template system for tint screen
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
      command->color.r = r;
      command->color.g = g;
      command->color.b = b;
      command->color.intensity = m_intensity;
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
