#include "Core/CommonUI/ColorTonePicker.hpp"
#include "Core/Application.hpp"
#include "imgui.h"

bool ColorTonePicker::draw() {
  ImGui::BeginGroup();
  {
    ImGui::BeginGroup();
    {
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 5.f);
      ImGui::TextUnformatted(trNOOP("Red:"));
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 12.f);
      ImGui::TextUnformatted(trNOOP("Green:"));
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 14.f);
      ImGui::TextUnformatted(trNOOP("Blue:"));
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 12.f);
      ImGui::TextUnformatted(trNOOP("Gray:"));
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::SetNextItemWidth(150);
      if (ImGui::SliderInt(std::format("##color_preview_red_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_r, -255, 255)) {
        m_toneInvalid = m_modified = true;
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(120);
      if (ImGui::InputInt("##color_preview_int_red", &m_r, 1, 100)) {
        m_toneInvalid = m_modified = true;
      }
      ImGui::SetNextItemWidth(150);
      if (ImGui::SliderInt(std::format("##color_preview_green_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_g, -255, 255)) {
        m_toneInvalid = m_modified = true;
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(120);
      if (ImGui::InputInt("##color_preview_int_green", &m_g, 1, 100)) {
        m_toneInvalid = m_modified = true;
      }
      ImGui::SetNextItemWidth(150);
      if (ImGui::SliderInt(std::format("##color_preview_blue_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_b, -255, 255)) {
        m_toneInvalid = m_modified = true;
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(120);
      if (ImGui::InputInt("##color_preview_int_blue", &m_b, 1, 100)) {
        m_toneInvalid = m_modified = true;
      }
      ImGui::SetNextItemWidth(150);
      if (ImGui::SliderInt(std::format("##color_preview_gray_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_gray, 0, 255)) {
        m_toneInvalid = m_modified = true;
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(120);
      if (ImGui::InputInt("##color_preview_int_gray", &m_gray, 1, 100)) {
        m_toneInvalid = m_modified = true;
      }
    }

    if (m_toneInvalid) {
      m_tonePreview.update(m_r / 255.f, m_g / 255.f, m_b / 255.f, m_gray / 255.f);
      m_toneInvalid = false;
    }

    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginChild(std::format("##color_preview_child_{}", reinterpret_cast<intptr_t>(this)).c_str(), ImVec2(0, 0),
                      ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle);
    { ImGui::Image(m_tonePreview, ImVec2{static_cast<float>(m_tonePreview.width()), static_cast<float>(m_tonePreview.height())}); }
    ImGui::EndChild();
  }
  ImGui::EndGroup();

  return m_modified;
}

void ColorTonePicker::setPreviewSize(int width, int height) {

  if (width == m_tonePreview.width() && height == m_tonePreview.height()) {
    return;
  }
  m_tonePreview.setSize(width, height);
}
