#include "Core/CommonUI/ColorFlashPicker.hpp"
#include "Core/Application.hpp"
#include "imgui.h"
#include "imgui_internal.h"

bool ColorFlashPicker::draw() {
  if (m_toneInvalid) {
    m_tonePreview.update(0.f, 0.f, 0.f, 0.f);
    m_toneInvalid = false;
  }
  ImGui::BeginGroup();
  {
    ImGui::BeginGroup();
    {
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 5.f);
      ImGui::Text(trNOOP("Red:"));
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 12.f);
      ImGui::Text(trNOOP("Green:"));
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 14.f);
      ImGui::Text(trNOOP("Blue:"));
      ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 12.f);
      ImGui::Text(trNOOP("Intensity:"));
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::SetNextItemWidth(150);
      if (ImGui::SliderInt(std::format("##color_preview_red_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_r, 0, 255)) {
        m_modified = true;
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(120);
      if (ImGui::InputInt("##color_preview_int_red", &m_r, 1, 100)) {
        m_toneInvalid = m_modified = true;
      }
      ImGui::SetNextItemWidth(150);
      if (ImGui::SliderInt(std::format("##color_preview_green_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_g, 0, 255)) {
        m_modified = true;
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(120);
      if (ImGui::InputInt("##color_preview_int_green", &m_g, 1, 100)) {
        m_toneInvalid = m_modified = true;
      }
      ImGui::SetNextItemWidth(150);
      if (ImGui::SliderInt(std::format("##color_preview_blue_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_b, 0, 255)) {
        m_modified = true;
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(120);
      if (ImGui::InputInt("##color_preview_int_blue", &m_b, 1, 100)) {
        m_toneInvalid = m_modified = true;
      }
      ImGui::SetNextItemWidth(150);
      if (ImGui::SliderInt(std::format("##color_preview_intensity_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_intensity, 0, 255)) {
        m_modified = true;
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(120);
      if (ImGui::InputInt("##color_preview_int_intensity", &m_intensity, 1, 100)) {
        m_toneInvalid = m_modified = true;
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginChild(std::format("##color_preview_child_{}", reinterpret_cast<intptr_t>(this)).c_str(), ImVec2(0, 0),
                      ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle);
    {
      ImGui::Dummy(ImVec2{static_cast<float>(m_tonePreview.width()), static_cast<float>(m_tonePreview.height())});
      auto win = ImGui::GetCurrentWindow();
      win->DrawList->AddImage(m_tonePreview, win->ContentRegionRect.Min, win->ContentRegionRect.Min + ImVec2{static_cast<float>(m_tonePreview.width()), static_cast<float>(m_tonePreview.height())});
      win->DrawList->AddRectFilled(win->ContentRegionRect.Min, win->ContentRegionRect.Min + ImVec2{static_cast<float>(m_tonePreview.width()), static_cast<float>(m_tonePreview.height())},
                                   IM_COL32(m_r, m_g, m_b, m_intensity));
    }
    ImGui::EndChild();
  }
  ImGui::EndGroup();

  return m_modified;
}

void ColorFlashPicker::setPreviewSize(int width, int height) {
  if (width == m_tonePreview.width() && height == m_tonePreview.height()) {
    return;
  }
  m_tonePreview.setSize(width, height);
}
