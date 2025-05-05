#include "Core/CommonUI/GameWindowColorPicker.hpp"

#include "GroupBox.hpp"
#include "orei18n.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui_internal.h"

#include <format>

bool GameWindowColorPicker::draw() {

  ImGui::BeginVertical("##window_color_vert_layout", ImGui::GetContentRegionAvail());
  {
    GroupBox operationRedGroupBox(trNOOP("Red"), "##change_window_redlayout", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
    ImGui::BeginVertical("##window_color_horz_layout_sliders", {-1, -1});
    {
      if (operationRedGroupBox.begin()) {

        ImGui::SetNextItemWidth(130);
        if (ImGui::SliderInt(std::format("##window_color_red_{}", reinterpret_cast<intptr_t>(this)).data(), &m_r, -255, 255)) {
          m_backgroundInvalid = m_modified = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        if (ImGui::InputInt(std::format("##window_color_red_int_{}", reinterpret_cast<intptr_t>(this)).data(), &m_r, 1, 100)) {
          m_backgroundInvalid = m_modified = true;
        }
      }
      operationRedGroupBox.end();
      GroupBox operationGreenGroupBox(trNOOP("Green"), "##change_window_greenlayout", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

      if (operationGreenGroupBox.begin()) {

        ImGui::SetNextItemWidth(130);
        if (ImGui::SliderInt(std::format("##window_color_green_{}", reinterpret_cast<intptr_t>(this)).data(), &m_g, -255, 255)) {
          m_backgroundInvalid = m_modified = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        if (ImGui::InputInt(std::format("##window_color_green_int_{}", reinterpret_cast<intptr_t>(this)).data(), &m_g, 1, 100)) {
          m_backgroundInvalid = m_modified = true;
        }
      }
      operationGreenGroupBox.end();
      GroupBox operationBlueGroupBox(trNOOP("Blue"), "##change_window_blue_layout", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

      if (operationBlueGroupBox.begin()) {

        ImGui::SetNextItemWidth(130);
        if (ImGui::SliderInt(std::format("##window_color_blue_{}", reinterpret_cast<intptr_t>(this)).data(), &m_b, -255, 255)) {
          m_backgroundInvalid = m_modified = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        if (ImGui::InputInt(std::format("##window_color_blue_int_{}", reinterpret_cast<intptr_t>(this)).data(), &m_b, 1, 100)) {
          m_backgroundInvalid = m_modified = true;
        }
      }
      operationBlueGroupBox.end();
    }
    ImGui::EndVertical();
    // ImGui::BeginHorizontal("change_window_image", {-1, -1});
    //{ ImGui::Image(m_background, {static_cast<float>(m_background.width()), static_cast<float>(m_background.height())}); }
    // ImGui::EndHorizontal();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
    ImGui::BeginHorizontal("##change_formation_dialog_buttons", {-1, -1});
    {
      ImGui::Spring();
      if (const auto ret = ImGui::ButtonGroup("##change_formation_dialog_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {

        // m_confirmed = true;
        // command->r = red;
        // command->g = green;
        // command->b = blue;
        // ImGui::CloseCurrentPopup();
        // setOpen(false);
      } else if (ret == 1) {
        // ImGui::CloseCurrentPopup();
        // setOpen(false);
      }
    }
    ImGui::EndHorizontal();
  }
  ImGui::EndVertical();

  /*
  ImGui::BeginGroup();
  {
    ImGui::BeginGroup();
    {
      ImGui::NewLine();
      ImGui::BeginGroup();
      {
        ImGui::Text("Red");
        ImGui::Spacing();
        ImGui::Text("Green");
        ImGui::Spacing();
        ImGui::Text("Blue");
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        if (ImGui::SliderInt(std::format("##window_color_red_{}", reinterpret_cast<intptr_t>(this)).data(), &m_r, -255, 255)) {
          m_backgroundInvalid = m_modified = true;
        }
        if (ImGui::SliderInt(std::format("##window_color_green_{}", reinterpret_cast<intptr_t>(this)).data(), &m_g, -255, 255)) {
          m_backgroundInvalid = m_modified = true;
        }
        if (ImGui::SliderInt(std::format("##window_color_blue_{}", reinterpret_cast<intptr_t>(this)).data(), &m_b, -255, 255)) {
          m_backgroundInvalid = m_modified = true;
        }
      }
      ImGui::EndGroup();

      if (m_backgroundInvalid) {
        m_background.update(m_r, m_g, m_b);
        m_backgroundInvalid = false;
      }
    }
    ImGui::EndGroup();

  }
  ImGui::EndGroup();
*/
  return m_modified;
}

void GameWindowColorPicker::setPreviewSize(int width, int height) {
  if (width == m_background.width() && height == m_background.height()) {
    return;
  }
  m_background.setSize(width, height);
}
