#include "Core/DatabaseEditor/DBTemplatesTab.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "Core/MainWindow.hpp"
#include "Database/Templates.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cstring>

void DBTemplatesTab::draw() {

  ImGui::BeginChild("##orpg_templates_editor");
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.f);
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Add", ImVec2{App::DPIHandler::scale_value(300), 0})) {

        AddTemplate("New Template", Template::TemplateType::Command, "", {});
        m_selection = m_templates->templates.size() > 0 ? m_templates->templates.size() - 1 : 0;
        SetTemplate();
        SaveToFile();
      }
      ImGui::SameLine();
      ImGui::BeginDisabled(m_templates->templates.size() == 0);
      if (ImGui::Button("Delete", ImVec2{App::DPIHandler::scale_value(300), 0})) {
        const int start = m_templates->templates.at(m_selection).id();
        if (m_templates->templates.size() == 1) {
          m_templates->templates.clear();
        } else {
          if (start >= 0) {
            m_templates->templates.erase(m_templates->templates.begin() + (start - 1));
          }
        }

        for (int idx{1}; auto& templ : m_templates->templates) {
          templ.setId(idx);
          idx++; // Reorder the indexes
        }
        m_selection = m_selection == m_templates->templates.size() ? (m_selection - 1) : m_selection;
        if (m_templates->templates.size() > 0) {
          SetTemplate();
        }
        SaveToFile();
      }
      ImGui::EndDisabled();
      ImGui::SameLine();

      ImGui::BeginDisabled(!(m_hasChanges || m_currentTemplate->hasChanges() || m_templateType == 0) || m_templates->templates.size() < 1);
      if (ImGui::Button("Apply", ImVec2{App::DPIHandler::scale_value(300), 0})) {
        SaveChanges();
        if (m_templates->serialize(m_parent->project()->database().basePath + "data/Templates.json")) {
          ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, "Serialized data/Templates.json successfully!"});
        } else {
          ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, "Failed to serialize data/Templates.json!"});
        }
        m_hasChanges = false;
      }
      ImGui::EndDisabled();
      // Template List
      if (ImGui::BeginCombo("##orpg_database_templates_templatelist", m_selection < m_templates->templates.size() ? m_templates->templates.at(m_selection).name().c_str() : "")) {
        int index{0};
        for (auto& templates : m_templates->templates) {
          if (const bool is_selected = m_selection == index; ImGui::Selectable(m_templates->templates.at(index).name().c_str(), is_selected)) {
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

    if (!m_templates->templates.empty()) {
      ImGui::SeparatorText("Template Properties");
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(10));
      ImGui::Text("Name:");
      ImGui::SameLine();
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(700));
      ImGui::Text("Type:");
      // TemplateType List
      ImGui::BeginGroup();
      {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(10));
        if (ImGui::InputText("##orpg_templates_name_input", &m_templateName)) {
          if (m_templateName == m_templates->templates.at(m_selection).name()) {
            m_hasChanges = false;
          } else {
            m_hasChanges = true;
          }
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
        ImGui::BeginDisabled(m_templateType == 2); // Shouldn't let event templates change
        if (ImGui::BeginCombo("##orpg_templatetypes_list", DecodeEnumName(magic_enum::enum_name(static_cast<Template::TemplateType>(m_templateType))).c_str())) {
          int index{0};
          for (auto& dir : magic_enum::enum_values<Template::TemplateType>()) {

            if (magic_enum::enum_index(dir).value() == 2) {
              continue;
            } // Skip event val

            bool is_selected = m_templateType == magic_enum::enum_index(dir).value();
            if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
              m_templateType = magic_enum::enum_index(dir).value();
              m_currentTemplate = CreateTemplateDialog(static_cast<Template::TemplateType>(m_templateType));
              if (m_templateType == static_cast<int>(m_templates->templates.at(m_selection).type())) {
                m_hasChanges = false;
              } else {
                m_hasChanges = true;
              }
              if (is_selected)
                ImGui::SetItemDefaultFocus();
            }
            index++;
          }
          ImGui::EndCombo();
        }
        ImGui::EndDisabled();
      }
      ImGui::EndGroup();
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.f);
      ImGui::BeginGroup();
      {
        ImGui::Text("Note:");
        char note[8192];
        strncpy(note, m_templateNote.c_str(), IM_ARRAYSIZE(note));
        if (ImGui::InputTextMultiline("##orpg_templates_note_input", note, IM_ARRAYSIZE(note), ImVec2{App::DPIHandler::scale_value(875), App::DPIHandler::scale_value(150)})) {
          m_templateNote = note;
        }
        ImGui::EndGroup();
      }
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(10));
      ImGui::BeginGroup();
      {
        if (m_templateType == 0) {
          m_commandEditor.draw();
        } else {
          m_currentTemplate->draw();
        }
        ImGui::EndGroup();
      }
    }
  }
  ImGui::EndChild();
}
void DBTemplatesTab::SetTemplate() {
  m_id = m_templates->templates.at(m_selection).id();
  m_templateName = m_templates->templates.at(m_selection).name();
  m_templateNote = m_templates->templates.at(m_selection).note();
  m_templateType = static_cast<int>(m_templates->templates.at(m_selection).type());
  m_currentTemplate = CreateTemplateDialog(static_cast<Template::TemplateType>(m_templateType));

  if (m_templates->templates.at(m_selection).commands() == "") {
    m_currentCommands.clear();
    m_currentCommands.emplace_back(std::make_shared<EventDummy>());
    m_currentCommands.back()->indent = 0;
  } else {
    if (m_templateType == 0) {
      CommandParser parser;
      nlohmann::ordered_json cmdJson = nlohmann::ordered_json::parse(m_templates->templates.at(m_selection).commands());
      m_currentCommands = parser.parse(cmdJson);
    } else {
      m_currentCommands.clear(); // Everything else has no commands
    }
  }
  m_hasChanges = false;
}

void DBTemplatesTab::SaveChanges() {
  m_templates->templates.at(m_selection).setName(m_templateName);
  m_templates->templates.at(m_selection).setNote(m_templateNote);
  m_templates->templates.at(m_selection).setType(static_cast<Template::TemplateType>(m_templateType));
  if (m_templates->templates.at(m_selection).type() == Template::TemplateType::Command) {
    m_templates->templates.at(m_selection).setParameters({});
    nlohmann::ordered_json cmdJson;
    CommandParser::serialize(cmdJson, m_currentCommands);
    m_templates->templates.at(m_selection).setCommands(cmdJson.dump());
  } else if (m_templates->templates.at(m_selection).type() == Template::TemplateType::Tint) {
    nlohmann::ordered_json cmdJson;
    CommandParser::serialize(cmdJson, m_currentCommands);
    m_templates->templates.at(m_selection).setCommands(cmdJson.dump());
    m_templates->templates.at(m_selection).setParameters(m_currentTemplate->getData());
  }
}
void DBTemplatesTab::AddTemplate(std::string label, Template::TemplateType type, std::string commandString, std::vector<int> params) {

  m_currentCommands.clear();
  m_currentCommands.emplace_back(std::make_shared<EventDummy>());
  m_currentCommands.back()->indent = 0;
  m_commandEditor.setCommands(&m_currentCommands);

  m_templates->addTemplate(Template(m_templates->templates.size() + 1, label + " " + std::to_string(m_templates->templates.size() + 1), "", type, commandString, params));

  m_id = m_templates->templates.back().id();
  m_templateName = m_templates->templates.back().name();
  m_templateType = static_cast<int>(m_templates->templates.back().type());

  nlohmann::ordered_json cmdJson;
  CommandParser::serialize(cmdJson, m_currentCommands);
  m_templates->templates.at(m_templates->templates.size() - 1).setCommands(cmdJson.dump());
}

void DBTemplatesTab::SaveToFile() {
  if (m_templates->serialize(m_parent->project()->database().basePath + "data/Templates.json")) {
    if (m_templates->serialize(Database::instance()->basePath + "data/Templates.json")) {
      ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, "Serialized data/Templates.json successfully!"});
    } else {
      ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, "Failed to serialize data/Templates.json!"});
    }
  }
}