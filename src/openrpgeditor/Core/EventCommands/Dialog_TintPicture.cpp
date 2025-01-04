#include "Core/EventCommands/Dialog_TintPicture.hpp"

#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_TintPicture::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{400, 290}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Picture");
    ImGui::SetNextItemWidth(75);
    if (ImGui::InputInt("##tintpicture_picture_id", &m_picture, 1, 100)) {
      if (m_picture < 1)
        m_picture = 1;
      if (m_picture > 999)
        m_picture = 999;
    }
    ImGui::SeparatorText("Color Tone");
    ImGui::BeginGroup();
    {
      ImGui::Text("Red:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Green:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Blue:");
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
      ImGui::Text("Gray:");
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##tintpicture_red", &r, -255, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(75);
      if (ImGui::InputInt("##tintpicture_int_red", &r, 1, 100)) {
        if (r < -255)
          r = -255;
        if (r > 255)
          r = 255;
      }

      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##tintpicture_green", &g, -255, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(75);
      if (ImGui::InputInt("##tintpicture_int_green", &g, 1, 100)) {
        if (g < -255)
          g = -255;
        if (g > 255)
          g = 255;
      }

      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##tintpicture_blue", &b, -255, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(75);
      if (ImGui::InputInt("##tintpicture_int_blue", &b, 1, 100)) {
        if (b < -255)
          b = -255;
        if (b > 255)
          b = 255;
      }

      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##tintpicture_gray", &gray, 0, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(75);
      if (ImGui::InputInt("##tintpicture_int_gray", &gray, 1, 100)) {
        if (gray < 1)
          gray = 1;
        if (gray > 255)
          gray = 255;
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();

    ImGui::ColorButton("##tintpicture_square", ImVec4{static_cast<float>(r * (1.0f / 255.0f)), static_cast<float>(g * (1.0f / 255.0f)), static_cast<float>(b * (1.0f / 255.0f)), 1}, 0,
                       ImVec2{100, 100});

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.f);
    ImGui::PushItemWidth(380);
    if (ImGui::BeginCombo("##tintpicture_presets", "Future presets go in this list")) {
      // TODO: Template system for tint screen
      ImGui::EndCombo();
    }
    ImGui::SeparatorText("Duration");
    ImGui::SetNextItemWidth(100);
    if (ImGui::InputInt("##tintpicture_duration", &m_duration)) {
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
      command->color.gray = gray;
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
