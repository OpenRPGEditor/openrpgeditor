#include "Core/EventCommands/Dialog_SetWeatherEffect.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"

std::tuple<bool, bool> Dialog_SetWeatherEffect::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{400, 165} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Weather");
    ImGui::BeginGroup();
    {
      ImGui::Text("Type:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Power:");
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::PushItemWidth((App::DPIHandler::scale_value(160)));
      if (ImGui::BeginCombo("##weather_effect_selection",
                            DecodeEnumName(magic_enum::enum_value<WeatherEffect>(m_effect)).c_str())) {
        for (auto& effect : magic_enum::enum_values<WeatherEffect>()) {
          bool is_selected = m_effect == magic_enum::enum_index(effect).value();
          if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(effect)).c_str(), is_selected)) {
            m_effect = magic_enum::enum_index(effect).value();
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::PushItemWidth((App::DPIHandler::scale_value(160)));
      ImGui::SliderInt("##weather_effect_power", &m_power, 1, 9, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::PushItemWidth((App::DPIHandler::scale_value(80)));
      if (ImGui::InputInt("##weather_effect_power_int", &m_power)) {
        if (m_power > 9)
          m_power = 9;
        if (m_power < 1)
          m_power = 1;
      }
    }
    ImGui::EndGroup();
    ImGui::SeparatorText("Duration");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
    if (ImGui::InputInt("##tintscreen_duration", &m_duration)) {
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
      command->effect = static_cast<WeatherEffect>(m_effect);
      command->power = m_power;
      command->duration = m_duration;
      command->waitForCompletion = m_waitForCompletion;
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
