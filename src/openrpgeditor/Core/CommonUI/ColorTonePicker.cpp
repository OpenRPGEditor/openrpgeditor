#include "Core/CommonUI/ColorTonePicker.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

bool ColorTonePicker::draw() {
  ImGui::BeginVertical("##colortone_picker_main_layout", {}, 0);
  {
    // TODO: Add ability to save presets, also add a boolean to hide this where it's not necessary
    GroupBox presetsGroup(trNOOP("Presets"), "##colortone_picker_presets_group", {-1, 0});
    if (presetsGroup.begin()) {
      ImGui::BeginHorizontal("##colortone_picker_presets_layout", {}, 0);
      {
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * .70f);
        if (ImGui::BeginCombo("##colortone_presets", m_currentTemplate == -1 ? trNOOP("None") : Database::instance()->templates.templates.at(m_currentTemplate).name().c_str())) {
          for (int index{0}; auto& templ : Database::instance()->templates.templateList(Template::TemplateType::Tint)) {
            if (!templ.parameters().empty()) {
              bool selected = m_currentTemplate == index;
              if (ImGui::Selectable(templ.name().c_str(), selected)) {
                m_currentTemplate = index;
                m_r = templ.parameters().at(0);
                m_g = templ.parameters().at(1);
                m_b = templ.parameters().at(2);
                m_gray = templ.parameters().at(3);
                m_toneInvalid = m_modified = true;
              }
              if (selected)
                ImGui::SetItemDefaultFocus();
              index++;
            } else {
              if (ImGui::Selectable(("Error loading template: " + templ.name()).c_str(), false)) {}
            }
          }
          ImGui::EndCombo();
        }
        if (ImGui::Button(trNOOP("Save as preset"), {-1, 0})) {
          // TODO(nexus): Implement name dialog
        }
      }
      ImGui::EndHorizontal();
    }
    presetsGroup.end();
    ImGui::BeginHorizontal("##colortone_picker_inner_layout", {ImGui::GetContentRegionAvail().x, 0}, 0);
    {
      ImGui::BeginVertical("##colortone_picker_right_layout", {}, 0);
      {
        GroupBox redGroup(trNOOP("Red"), "##colortone_preview_red_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
        if (redGroup.begin()) {
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
          if (ImGui::SliderInt("##colortone_preview_red_slider", &m_r, -255, 255)) {
            m_r = std::clamp(m_r, -255, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(-1);
          if (ImGui::SpinInt("##color_preview_int_red", &m_r, 1, 100)) {
            m_r = std::clamp(m_r, -255, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
        }
        redGroup.end();

        GroupBox greenGroup(trNOOP("Green"), "##colortone_preview_green_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
        if (greenGroup.begin()) {
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
          if (ImGui::SliderInt("##color_preview_green_slider", &m_g, -255, 255)) {
            m_g = std::clamp(m_g, -255, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(-1);
          if (ImGui::SpinInt("##color_preview_int_green", &m_g, 1, 100)) {
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
        }
        greenGroup.end();
        GroupBox blueGroup(trNOOP("Blue"), "##colortone_preview_blue_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
        if (blueGroup.begin()) {
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
          if (ImGui::SliderInt("##color_preview_blue_slider", &m_b, -255, 255)) {
            m_b = std::clamp(m_b, -255, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(-1);
          if (ImGui::SpinInt("##color_preview_int_blue", &m_b, 1, 100)) {
            m_b = std::clamp(m_b, -255, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
        }
        blueGroup.end();
        GroupBox grayGroup(trNOOP("Gray"), "##colortone_preview_gray_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
        if (grayGroup.begin()) {
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
          if (ImGui::SliderInt("##color_preview_gray_slider", &m_gray, 0, 255)) {
            m_gray = std::clamp(m_gray, 0, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(-1);
          if (ImGui::SpinInt("##color_preview_int_gray", &m_gray, 1, 100)) {
            m_gray = std::clamp(m_gray, 0, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
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
  }
  ImGui::EndVertical();

  return m_modified;
}

void ColorTonePicker::setPreviewSize(const int width, const int height) {

  if (width == m_tonePreview.width() && height == m_tonePreview.height()) {
    return;
  }
  m_tonePreview.setSize(width, height);
}
