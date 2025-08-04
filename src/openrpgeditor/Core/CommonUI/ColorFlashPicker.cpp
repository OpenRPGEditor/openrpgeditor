#include "Core/CommonUI/ColorFlashPicker.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

bool ColorFlashPicker::draw() {
  if (m_toneInvalid) {
    m_tonePreview.update(0.f, 0.f, 0.f, 0.f);
    m_toneInvalid = false;
  }

  ImGui::BeginVertical("##colorflash_picker_main_layout", {}, 0);
  {
    // TODO: Add ability to save presets, also add a boolean to hide this where it's not necessary
    GroupBox presetsGroup(trNOOP("Presets"), "##colorflash_picker_presets_group", {-1, 0});
    if (presetsGroup.begin()) {
      ImGui::BeginHorizontal("##colorflash_picker_presets_layout", {}, 0);
      {
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * .70f);
        if (ImGui::BeginCombo("##colorflash_presets", m_currentTemplate == -1 ? trNOOP("None") : Database::instance()->templates.templates.at(m_currentTemplate).name().c_str())) {
          for (int index{0}; auto& templ : Database::instance()->templates.templateList(Template::TemplateType::Flash)) {
            if (!templ.parameters().empty()) {
              bool selected = m_currentTemplate == index;
              if (ImGui::Selectable(templ.name().c_str(), selected)) {
                m_currentTemplate = index;
                m_r = templ.parameters().at(0);
                m_g = templ.parameters().at(1);
                m_b = templ.parameters().at(2);
                m_intensity = templ.parameters().at(3);
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
    ImGui::BeginHorizontal("##colorflash_picker_inner_layout", {ImGui::GetContentRegionAvail().x, 0}, 0);
    {
      ImGui::BeginVertical("##colorflash_picker_right_layout", {}, 0);
      {
        GroupBox redGroup(trNOOP("Red"), "##colorflash_preview_red_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
        if (redGroup.begin()) {
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
          if (ImGui::SliderInt("##colorflash_preview_red_slider", &m_r, 0, 255)) {
            m_r = std::clamp(m_r, 0, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(-1);
          if (ImGui::SpinInt("##color_preview_int_red", &m_r, 1, 100)) {
            m_r = std::clamp(m_r, 0, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
        }
        redGroup.end();

        GroupBox greenGroup(trNOOP("Green"), "##colorflash_preview_green_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
        if (greenGroup.begin()) {
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
          if (ImGui::SliderInt("##color_preview_green_slider", &m_g, 0, 255)) {
            m_g = std::clamp(m_g, 0, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(-1);
          if (ImGui::SpinInt("##color_preview_int_green", &m_g, 1, 100)) {
            m_g = std::clamp(m_g, 0, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
        }
        greenGroup.end();
        GroupBox blueGroup(trNOOP("Blue"), "##colorflash_preview_blue_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
        if (blueGroup.begin()) {
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
          if (ImGui::SliderInt("##color_preview_blue_slider", &m_b, 0, 255)) {
            m_b = std::clamp(m_b, -255, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(-1);
          if (ImGui::SpinInt("##color_preview_int_blue", &m_b, 1, 100)) {
            m_b = std::clamp(m_b, 0, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
        }
        blueGroup.end();
        GroupBox grayGroup(trNOOP("Intensity"), "##colorflash_preview_intensity_group", {ImGui::GetContentRegionAvail().x * .70f, 0});
        if (grayGroup.begin()) {
          ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
          if (ImGui::SliderInt("##color_preview_gray_slider", &m_intensity, 0, 255)) {
            m_intensity = std::clamp(m_intensity, 0, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(-1);
          if (ImGui::SpinInt("##color_preview_int_gray", &m_intensity, 1, 100)) {
            m_intensity = std::clamp(m_intensity, 0, 255);
            m_toneInvalid = m_modified = true;
            m_currentTemplate = -1;
          }
        }
        grayGroup.end();
      }
      ImGui::EndVertical();

      ImGui::BeginChild("##color_preview_child", ImGui::GetContentRegionAvail(),
                        ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle);
      {
        const auto size = ImGui::GetContentRegionAvail();
        ImGui::Dummy(size);
        auto win = ImGui::GetCurrentWindow();
        win->DrawList->AddImage(m_tonePreview, win->ContentRegionRect.Min, win->ContentRegionRect.Min + size);
        win->DrawList->AddRectFilled(win->ContentRegionRect.Min, win->ContentRegionRect.Min + size, IM_COL32(m_r, m_g, m_b, m_intensity));
      }
      ImGui::EndChild();
    }
    ImGui::EndHorizontal();
  }
  ImGui::EndVertical();

  return m_modified;
}

void ColorFlashPicker::setPreviewSize(const int width, const int height) {
  if (width == m_tonePreview.width() && height == m_tonePreview.height()) {
    return;
  }
  m_tonePreview.setSize(width, height);
}
