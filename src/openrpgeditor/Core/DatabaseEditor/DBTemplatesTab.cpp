#include "Core/DatabaseEditor/DBTemplatesTab.hpp"
#include "imgui.h"
#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "misc/cpp/imgui_stdlib.h"
#include "Core/Project.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Database/Templates.hpp"

void DBTemplatesTab::draw() {
  ImGui::BeginChild("##orpg_templates_editor");
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.f);
    ImGui::BeginGroup();
    {

      if (ImGui::Button("Add", ImVec2{App::DPIHandler::scale_value(300), 0})) {
        m_templates->templates.push_back(
            Template("New Template" + std::to_string(m_templates->templates.size()), 0, {}, {}));
        if (m_templates->serialize(m_parent->project()->database().basePath + "data/Templates.json")) {
          ImGui::InsertNotification(
              ImGuiToast{ImGuiToastType::Success, "Serialized data/Templates.json successfully!"});
        } else {
          ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, "Failed to serialize data/Templates.json!"});
        }
        m_selection = m_templates->templates.size() > 0 ? m_templates->templates.size() - 1 : 0;
      }
      ImGui::SameLine();
      if (ImGui::Button("Delete", ImVec2{App::DPIHandler::scale_value(300), 0})) {}
      ImGui::SameLine();

      ImGui::BeginDisabled(!m_hasChanges);
      if (ImGui::Button("Apply", ImVec2{App::DPIHandler::scale_value(300), 0})) {
        SaveChanges();
        if (m_templates->serialize(m_parent->project()->database().basePath + "data/Templates.json")) {
          ImGui::InsertNotification(
              ImGuiToast{ImGuiToastType::Success, "Serialized data/Templates.json successfully!"});
        } else {
          ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, "Failed to serialize data/Templates.json!"});
        }
        m_hasChanges = false;
      }
      ImGui::EndDisabled();

      // Template List
      if (ImGui::BeginCombo(
              "##orpg_database_templates_templatelist",
              m_selection < m_templates->templates.size() ? m_templates->templates.at(m_selection).name.c_str() : "")) {
        int index{0};
        for (auto& templates : m_templates->templates) {
          bool is_selected = m_selection == index;
          if (ImGui::Selectable(m_templates->templates.at(index).name.c_str(), is_selected)) {
            m_selection = index;
            SetTemplate();
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
          index++;
        }
        ImGui::EndCombo();
      }
    }
    ImGui::EndGroup(); // End Group (Add, Delete, Edit, TemplateList)
    ImGui::SeparatorText("Template Properties");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(10));
    ImGui::Text("Name:");
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(500));
    ImGui::Text("Type:");
    // TemplateType List
    ImGui::BeginGroup();
    {
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(10));
      ImGui::InputText("##orpg_templates_name_input", &m_templateName);
      ImGui::SameLine();
      ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
      if (ImGui::BeginCombo(
              "##orpg_templatetypes_list",
              DecodeEnumName(magic_enum::enum_name(static_cast<Template::TemplateType>(m_templateType))).c_str())) {
        int index{0};
        for (auto& dir : magic_enum::enum_values<Template::TemplateType>()) {
          bool is_selected = m_templateType == magic_enum::enum_index(dir).value();
          if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
            m_templateType = magic_enum::enum_index(dir).value();
            m_currentTemplate = CreateTemplateDialog(static_cast<Template::TemplateType>(m_templateType));
            m_hasChanges = true;
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
          index++;
        }
        ImGui::EndCombo();
      }
    }
    ImGui::EndGroup();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(10));
    ImGui::BeginGroup();
    {
      if (m_currentTemplate) {
        m_currentTemplate->draw();
      }
      ImGui::EndGroup();
    }
  }
  ImGui::EndChild();
}