#include "Core/CommonUI/GameWindowColorPicker.hpp"
#include "Core/DPIHandler.hpp"
#include <format>

bool GameWindowColorPicker::draw() {
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
      if (ImGui::SliderInt(std::format("##window_color_red_{}", reinterpret_cast<intptr_t>(this)).data(), &m_r, -255,
                           255)) {
        m_backgroundInvalid = m_modified = true;
      }
      if (ImGui::SliderInt(std::format("##window_color_green_{}", reinterpret_cast<intptr_t>(this)).data(), &m_g, -255,
                           255)) {
        m_backgroundInvalid = m_modified = true;
      }
      if (ImGui::SliderInt(std::format("##window_color_blue_{}", reinterpret_cast<intptr_t>(this)).data(), &m_b, -255,
                           255)) {
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
  ImGui::SameLine();
  ImGui::BeginChild(std::format("##window_color_preview_child_{}", reinterpret_cast<intptr_t>(this)).data(), {},
                    ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                        ImGuiChildFlags_AlwaysAutoResize);
  { ImGui::Image(m_background, {static_cast<float>(m_background.width()), static_cast<float>(m_background.height())}); }
  ImGui::EndChild();

  return m_modified;
}

void GameWindowColorPicker::setPreviewSize(int width, int height) {
  width *= App::DPIHandler::get_ui_scale();
  height *= App::DPIHandler::get_ui_scale();

  if (width == m_background.width() && height == m_background.height()) {
    return;
  }
  m_background.setSize(width, height);
}
