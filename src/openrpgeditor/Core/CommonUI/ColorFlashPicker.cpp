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
      ImGui::Text("Red");
      ImGui::Spacing();
      ImGui::Text("Green");
      ImGui::Spacing();
      ImGui::Text("Blue");
      ImGui::Spacing();
      ImGui::Text("Intensity");
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      if (ImGui::SliderInt(std::format("##color_preview_red_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_r, 0, 255)) {
        m_modified = true;
      }
      if (ImGui::SliderInt(std::format("##color_preview_green_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_g, 0, 255)) {
        m_modified = true;
      }
      if (ImGui::SliderInt(std::format("##color_preview_blue_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_b, 0, 255)) {
        m_modified = true;
      }
      if (ImGui::SliderInt(std::format("##color_preview_intensity_slider_{}", reinterpret_cast<intptr_t>(this)).c_str(), &m_intensity, 0, 255)) {
        m_modified = true;
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
