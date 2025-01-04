#include "Core/TemplateEditor/TemplatesTint.hpp"
#include "Database/Templates.hpp"
#include "Core/Application.hpp"

#include "imgui.h"

void TemplatesTint::draw() {
  ImGui::BeginChild("#orpg_template_editor_tint");
  {
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
      ImGui::SliderInt("##tintscreen_red", &r, -255, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(100);
      if (ImGui::InputInt("##tintscreen_int_red", &r, 1, 100)) {
        if (r < -255)
          r = -255;
        if (r > 255)
          r = 255;
      }

      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##tintscreen_green", &g, -255, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(100);
      if (ImGui::InputInt("##tintscreen_int_green", &g, 1, 100)) {
        if (g < -255)
          g = -255;
        if (g > 255)
          g = 255;
      }

      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##tintscreen_blue", &b, -255, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(100);
      if (ImGui::InputInt("##tintscreen_int_blue", &b, 1, 100)) {
        if (b < -255)
          b = -255;
        if (b > 255)
          b = 255;
      }

      ImGui::SetNextItemWidth(150);
      ImGui::SliderInt("##tintscreen_gray", &gray, 0, 255, "", ImGuiSliderFlags_NoInput);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(100);
      if (ImGui::InputInt("##tintscreen_int_gray", &gray, 1, 100)) {
        if (gray < 1)
          gray = 1;
        if (gray > 255)
          gray = 255;
        // std::array<float, 3> hsv = rgbToHsv(r, g, b);
        // hsv[1] = gray / 100.f;
        // std::array<int, 3> rgb = hsvToRgb(hsv[0], hsv[1], hsv[2]);
        // r = rgb[0];
        // g = rgb[1];
        // b = rgb[2];
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();

    ImGui::ColorButton("##tintscreen_square", ImVec4{static_cast<float>(r * (1.0f / 255.0f)), static_cast<float>(g * (1.0f / 255.0f)), static_cast<float>(b * (1.0f / 255.0f)), 1}, 0,
                       ImVec2{100, 100});
  }
  ImGui::EndChild();
};