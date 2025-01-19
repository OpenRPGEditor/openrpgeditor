#include "Core/EventCommands/Dialog_TintScreen.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_TintScreen::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{400, 250}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::SeparatorText("Color Tone");
    color_picker.draw();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.f);
    ImGui::PushItemWidth(380);
    if (ImGui::BeginCombo("##tintscreen_presets", m_currentTemplate == -1 ? "" : Database::instance()->templates.templates.at(m_currentTemplate).name().c_str())) {
      int index{0};
      for (auto& templ : Database::instance()->templates.templateList(Template::TemplateType::Tint)) {
        if (!templ.parameters().empty()) {
          bool is_selected = m_currentTemplate == index;
          if (ImGui::Selectable(templ.name().c_str(), is_selected)) {
            m_currentTemplate = index;
            color_picker.setValues(templ.parameters().at(0), templ.parameters().at(1), templ.parameters().at(2), templ.parameters().at(3));

            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
          index++;
        } else {
          if (ImGui::Selectable(("Error loading template: " + templ.name()).c_str(), false)) {}
        }
      }
      ImGui::EndCombo();
    }
    ImGui::SeparatorText("Duration");
    ImGui::SetNextItemWidth(100);
    if (ImGui::InputInt("##tintscreen_duration", &m_duration)) {
      if (m_duration < 1)
        m_duration = 1;
      if (m_duration > 999)
        m_duration = 999;
    }
    ImGui::SameLine();
    ImGui::Text("frames 1/60 sec");
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 80.f);
    ImGui::Checkbox("Wait for Completion", &m_waitForCompletion);

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->color.r = color_picker.r();
      command->color.g = color_picker.g();
      command->color.b = color_picker.b();
      command->color.gray = color_picker.gray();
      command->duration = m_duration;
      command->waitForCompletion = m_waitForCompletion;
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
