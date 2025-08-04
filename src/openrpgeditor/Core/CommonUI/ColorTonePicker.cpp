#include "Core/CommonUI/ColorTonePicker.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

bool ColorTonePicker::draw() {
  ImGui::PushItemWidth(-1);
  if (ImGui::BeginCombo("##colortone_picker_presets", "Future presets go in this list")) {
    // TODO: Template system for tint screen
    ImGui::EndCombo();
  }
  ImGui::BeginHorizontal("##colortone_picker_main_layout", {ImGui::GetContentRegionAvail().x, 0}, 0);
  {
    ImGui::BeginVertical("##colortone_picker_right_layout", {}, 0);
    {
      GroupBox redGroup(trNOOP("Red"), "##colortone_preview_red_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
      if (redGroup.begin()) {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
        if (ImGui::SliderInt("##colortone_preview_red_slider", &m_r, -255, 255)) {
          m_toneInvalid = m_modified = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##color_preview_int_red", &m_r, 1, 100)) {
          m_toneInvalid = m_modified = true;
        }
      }
      redGroup.end();

      GroupBox greenGroup(trNOOP("Green"), "##colortone_preview_green_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
      if (greenGroup.begin()) {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
        if (ImGui::SliderInt("##color_preview_green_slider", &m_g, -255, 255)) {
          m_toneInvalid = m_modified = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##color_preview_int_green", &m_g, 1, 100)) {
          m_toneInvalid = m_modified = true;
        }
      }
      greenGroup.end();
      GroupBox blueGroup(trNOOP("Blue"), "##colortone_preview_blue_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
      if (blueGroup.begin()) {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
        if (ImGui::SliderInt("##color_preview_blue_slider", &m_b, -255, 255)) {
          m_toneInvalid = m_modified = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##color_preview_int_blue", &m_b, 1, 100)) {
          m_toneInvalid = m_modified = true;
        }
      }
      blueGroup.end();
      GroupBox grayGroup(trNOOP("Gray"), "##colortone_preview_gray_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
      if (grayGroup.begin()) {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
        if (ImGui::SliderInt("##color_preview_gray_slider", &m_gray, 0, 255)) {
          m_toneInvalid = m_modified = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##color_preview_int_gray", &m_gray, 1, 100)) {
          m_toneInvalid = m_modified = true;
        }
      }
      grayGroup.end();
    }
    ImGui::EndVertical();

    if (m_toneInvalid) {
      m_tonePreview.update(m_r / 255.f, m_g / 255.f, m_b / 255.f, m_gray / 255.f);
      m_toneInvalid = false;
    }
    ImGui::BeginChild("##color_preview_child", ImGui::GetContentRegionAvail(),
                      ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle);
    { ImGui::Image(m_tonePreview, ImGui::GetContentRegionAvail()); }
    ImGui::EndChild();
  }
  ImGui::EndHorizontal();

  return m_modified;
}

void ColorTonePicker::setPreviewSize(const int width, const int height) {

  if (width == m_tonePreview.width() && height == m_tonePreview.height()) {
    return;
  }
  m_tonePreview.setSize(width, height);
}
