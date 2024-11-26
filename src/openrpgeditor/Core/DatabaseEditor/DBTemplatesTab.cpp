#include "Core/DatabaseEditor/DBTemplatesTab.hpp"
#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiExt/ImGuiNotify.hpp"
#include "Core/Project.hpp"
#include "Database/Templates.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
void DBTemplatesTab::draw() {

  ImGui::BeginChild("##orpg_templates_editor");
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.f);
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Add", ImVec2{App::DPIHandler::scale_value(300), 0})) {

        AddTemplate("New Template", Template::TemplateType::Command, "", {});
        if (m_templates->serialize(m_parent->project()->database().basePath + "data/Templates.json")) {
          ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, "Serialized data/Templates.json successfully!"});
          if (m_templates->serialize(Database::instance()->basePath + "data/Templates.json")) {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, "Serialized data/Templates.json successfully!"});
          } else {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, "Failed to serialize data/Templates.json!"});
          }
          m_selection = m_templates->templates.size() > 0 ? m_templates->templates.size() - 1 : 0;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete", ImVec2{App::DPIHandler::scale_value(300), 0})) {}
        ImGui::SameLine();

        ImGui::BeginDisabled(!(m_hasChanges || m_currentTemplate->hasChanges() || m_templateType == 0));
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
        if (ImGui::BeginCombo("##orpg_database_templates_templatelist", m_selection < m_templates->templates.size() ? m_templates->templates.at(m_selection).name.c_str() : "")) {
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

      if (!m_templates->templates.empty()) {
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
          if (ImGui::InputText("##orpg_templates_name_input", &m_templateName)) {
            if (m_templateName == m_templates->templates.at(m_selection).name) {
              m_hasChanges = false;
            } else {
              m_hasChanges = true;
            }
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(App::DPIHandler::scale_value(200));
          if (ImGui::BeginCombo("##orpg_templatetypes_list", DecodeEnumName(magic_enum::enum_name(static_cast<Template::TemplateType>(m_templateType))).c_str())) {
            int index{0};
            for (auto& dir : magic_enum::enum_values<Template::TemplateType>()) {
              bool is_selected = m_templateType == magic_enum::enum_index(dir).value();
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                m_templateType = magic_enum::enum_index(dir).value();
                m_currentTemplate = CreateTemplateDialog(static_cast<Template::TemplateType>(m_templateType));
                if (m_templateType == static_cast<int>(m_templates->templates.at(m_selection).type)) {
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
        }
        ImGui::EndGroup();
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
}
  void DBTemplatesTab::SetTemplate() {
    m_id = m_templates->templates.at(m_selection).id;
    m_templateName = m_templates->templates.at(m_selection).name;
    m_templateType = static_cast<int>(m_templates->templates.at(m_selection).type);
    m_currentTemplate = CreateTemplateDialog(static_cast<Template::TemplateType>(m_templateType));

    if (m_templates->templates.at(m_selection).commands == "") {
      m_currentCommands.clear();
      m_currentCommands.emplace_back(std::make_shared<EventDummy>());
      m_currentCommands.back()->indent = 0;
    } else {
      CommandParser parser;
      nlohmann::ordered_json cmdJson = nlohmann::ordered_json::parse(m_templates->templates.at(m_selection).commands);
      m_currentCommands = parser.parse(cmdJson);
    }
    m_hasChanges = false;
  }

  void DBTemplatesTab::SaveChanges() {
    m_templates->templates.at(m_selection).name = m_templateName;
    m_templates->templates.at(m_selection).type = static_cast<Template::TemplateType>(m_templateType);
    if (m_templates->templates.at(m_selection).type == Template::TemplateType::Command) {
      m_templates->templates.at(m_selection).parameters.clear();
      nlohmann::ordered_json cmdJson;
      CommandParser::serialize(cmdJson, m_currentCommands);
      m_templates->templates.at(m_selection).commands = cmdJson.dump();
    } else if (m_templates->templates.at(m_selection).type == Template::TemplateType::Tint) {
      // m_templates->templates.at(m_selection).commands.clear();
      nlohmann::ordered_json cmdJson;
      CommandParser::serialize(cmdJson, m_currentCommands);
      m_templates->templates.at(m_selection).commands = cmdJson.dump();
      m_templates->templates.at(m_selection).parameters = m_currentTemplate->getData();
    }
  }
  void DBTemplatesTab::AddTemplate(std::string label, Template::TemplateType type, std::string commandString, std::vector<int> params) {
    m_templates->addTemplate(Template(m_templates->templates.size() + 1, label + " " + std::to_string(m_templates->templates.size() + 1), type, commandString, params));

    m_id = m_templates->templates.back().id;
    m_templateName = m_templates->templates.back().name;
    m_templateType = static_cast<int>(m_templates->templates.back().type);
    m_currentCommands.clear();
    m_currentCommands.emplace_back(std::make_shared<EventDummy>());
    m_currentCommands.back()->indent = 0;

    nlohmann::ordered_json cmdJson;
    CommandParser::serialize(cmdJson, m_currentCommands);
    m_templates->templates.at(m_templates->templates.size() - 1).commands = cmdJson.dump();
  }