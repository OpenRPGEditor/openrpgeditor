#include "Core/CommonUI/GameWindowColorPicker.hpp"

#include "GroupBox.hpp"
#include "orei18n.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui_internal.h"

#include <format>

void GameWindowColorPicker::draw(ImVec2 size) {

  ImGui::BeginHorizontal("##window_color_horz_layout", size);
  {
    ImGui::BeginVertical("##window_col_sliders", {size.x * .6f, 0});
    {
      GroupBox operationRedGroupBox(trNOOP("Red"), "##window_col_red_group", {size.x * .6f, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationRedGroupBox.begin()) {

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .8f);
        if (ImGui::SliderInt("##window_color_red_slider", &m_r, -255, 255)) {
          m_backgroundInvalid = m_modified = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##window_color_red_spinbox", &m_r, 1, 100)) {
          m_r = std::clamp(m_r, -255, 255);
          m_backgroundInvalid = m_modified = true;
        }
      }
      operationRedGroupBox.end();
      ImGui::Spring(.50f);
      GroupBox operationGreenGroupBox(trNOOP("Green"), "##change_window_green_group", {size.x * .6f, 0}, nullptr,
                                      ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

      if (operationGreenGroupBox.begin()) {

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .8f);
        if (ImGui::SliderInt("##window_color_green_slider", &m_g, -255, 255)) {
          m_backgroundInvalid = m_modified = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##window_color_green_spinbox", &m_g, 1, 100)) {
          m_g = std::clamp(m_g, -255, 255);
          m_backgroundInvalid = m_modified = true;
        }
      }
      operationGreenGroupBox.end();
      ImGui::Spring(.50f);
      GroupBox operationBlueGroupBox(trNOOP("Blue"), "##change_window_blue_layout", {size.x * .6f, 0}, nullptr,
                                     ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

      if (operationBlueGroupBox.begin()) {

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .8f);
        if (ImGui::SliderInt("##window_color_blue_slider", &m_b, -255, 255)) {
          m_backgroundInvalid = m_modified = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##window_color_blue_spinbox", &m_b, 1, 100)) {
          m_b = std::clamp(m_b, -255, 255);
          m_backgroundInvalid = m_modified = true;
        }
      }
      operationBlueGroupBox.end();
    }
    ImGui::EndVertical();
    ImGui::BeginChild(ImGui::GetID("##window_color_preview"), {size.x * .4f, size.y},
                      ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle);
    {
      if (m_backgroundInvalid) {
        m_background.update(m_r, m_g, m_b);
        m_backgroundInvalid = false;
      }
      ImGui::Image(m_background, ImGui::GetContentRegionAvail());
    }
    ImGui::EndChildFrame();
  }
  ImGui::EndHorizontal();
}

void GameWindowColorPicker::setPreviewSize(int width, int height) {
  if (width == m_background.width() && height == m_background.height()) {
    return;
  }
  m_background.setSize(width, height);
}
