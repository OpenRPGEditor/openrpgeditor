#include "Core/EventCommands/Dialog_SetWeatherEffect.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_SetWeatherEffect::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###SetWeatherEffect");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 50) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 8) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###SetWeatherEffect", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    ImGui::BeginVertical("##set_weather_effect_main_layout", ImGui::GetContentRegionAvail(), 0.f);
    {
      GroupBox weatherGroupBox(trNOOP("Weather"), "##set_weather_effect_weather_group", {-1, 0});
      if (weatherGroupBox.begin()) {
        ImGui::BeginHorizontal("##set_weather_effect_weather_layout");
        {
          GroupBox typeGroup(trNOOP("Type"), "set_weather_effect_type_group", {ImGui::GetContentRegionAvail().x * 0.5f, 0});
          if (typeGroup.begin()) {
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##set_weather_effect_type_selection", DecodeEnumName(static_cast<WeatherEffect>(m_effect)).c_str())) {
              for (auto& effect : magic_enum::enum_values<WeatherEffect>()) {
                bool selected = m_effect == static_cast<int>(effect);
                if (ImGui::Selectable(DecodeEnumName(effect).c_str(), selected)) {
                  m_effect = static_cast<int>(effect);
                }
                if (selected) {
                  ImGui::SetItemDefaultFocus();
                }
              }
              ImGui::EndCombo();
            }
          }
          typeGroup.end();
          GroupBox powerGroup(trNOOP("Power"), "set_weather_effect_power", {-1, 0});
          if (powerGroup.begin()) {
            ImGui::SetNextItemWidth(-1);
            if (ImGui::SliderInt("##weather_effect_power_int", &m_power, 1, 9)) {
              m_power = std::clamp(m_power, 1, 9);
            }
          }
          powerGroup.end();
        }
        ImGui::EndHorizontal();
      }
      weatherGroupBox.end();
      GroupBox durationGroupBox(trNOOP("Duration"), "##set_weather_effect_duration_group", {-1, 0});
      if (durationGroupBox.begin()) {
        ImGui::BeginHorizontal("##set_weather_effect_duration_layout", {-1, 0});
        {
          // We need to calculate the spinbox size based on the text size for the following checkbox, this allows us to dynamically accommodate languages
          const auto checkboxLabel = trNOOP("Wait for Completion");
          const auto width = ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(checkboxLabel).x + ImGui::GetFrameHeightWithSpacing() + (ImGui::GetStyle().FramePadding.x * 2));
          ImGui::BeginChild("##set_weather_effect_duration", {0, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NavFlattened);
          {
            ImGui::SetNextItemWidth(width);
            if (ImGui::SpinInt("##set_weather_effect_duration", &m_duration, 1, 100, trNOOP("%d frames (1/60 sec)"))) {
              m_duration = std::clamp(m_duration, 1, 999);
            }
          }
          ImGui::EndChild();
          ImGui::Checkbox(checkboxLabel, &m_waitForCompletion);
        }
        ImGui::EndHorizontal();
      }
      durationGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##set_weather_effect_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##set_weather_effect_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->effect = static_cast<WeatherEffect>(m_effect);
          m_command->power = m_power;
          m_command->duration = m_duration;
          m_command->waitForCompletion = m_waitForCompletion;
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
          m_confirmed = false;
          setOpen(false);
          ImGui::CloseCurrentPopup();
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
