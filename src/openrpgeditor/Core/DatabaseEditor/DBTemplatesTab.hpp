#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/EventEditor.hpp"
#include "Core/TemplateEditor/IDBTemplates.hpp"
#include "Core/TemplateEditor/TemplatesCommand.hpp"
#include "Core/TemplateEditor/TemplatesTint.hpp"
#include "Database/Templates.hpp"

#include <optional>

struct Templates;
struct DBTemplatesTab : IDBEditorTab {
  explicit DBTemplatesTab(Templates& templates, DatabaseEditor* parent) : IDBEditorTab(parent), m_templates(&templates), m_commandEditor(nullptr) {
    if (m_templates->templates.size() > 0) {
      if (m_templates->templates.at(m_selection).commands.empty()) {
        m_currentCommands.emplace_back(std::make_shared<EventDummy>());
        m_currentCommands.back()->indent = 0;
      } else {
        CommandParser parser;
        nlohmann::ordered_json cmdJson = nlohmann::ordered_json::parse(m_templates->templates.at(m_selection).commands);
        m_currentCommands = parser.parse(cmdJson);
      }
      m_commandEditor.setCommands(&m_currentCommands);
      m_templateName = m_templates->templates.at(m_selection).name;
      m_templateType = static_cast<int>(m_templates->templates.at(m_selection).type);
      m_currentTemplate = CreateTemplateDialog(static_cast<Template::TemplateType>(m_templates->templates.at(m_selection).type));
    } else {
      m_currentTemplate = CreateTemplateDialog(static_cast<Template::TemplateType>(0));
      m_currentCommands.emplace_back(std::make_shared<EventDummy>());
      m_currentCommands.back()->indent = 0;
      m_commandEditor.setCommands(&m_currentCommands);
    }
  }
  void draw() override;
  std::vector<Templates*>& templates() { return m_template; }

private:
  Templates* m_templates = nullptr;
  std::vector<Templates*> m_template;
  std::vector<std::shared_ptr<IEventCommand>> m_currentCommands;
  int m_selection{0};
  int m_templateType{0};
  std::string m_templateName;
  bool m_hasChanges = false;
  EventCommandEditor m_commandEditor;

  std::shared_ptr<IDBTemplates> m_currentTemplate;

  std::shared_ptr<IDBTemplates> CreateTemplateDialog(Template::TemplateType type) {
    if (m_templates->templates.empty()) {
      return std::make_shared<TemplatesCommand>(nullptr, m_parent);
    } else {
      switch (type) {
      case Template::TemplateType::Command:
        return std::make_shared<TemplatesCommand>(&m_templates->templates.at(m_selection), m_parent);
      case Template::TemplateType::Tint:
        return std::make_shared<TemplatesTint>(&m_templates->templates.at(m_selection), m_parent);
      default:
        return nullptr;
      }
    }
  }
  void AddTemplate(std::string label, Template::TemplateType type, std::string commandString, std::vector<int> params);
  void SetTemplate();
  void SaveChanges();
};
