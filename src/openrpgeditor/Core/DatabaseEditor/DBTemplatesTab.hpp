#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/TemplateEditor/IDBTemplates.hpp"
#include "Database/Templates.hpp"
#include "Core/TemplateEditor/TemplatesTint.hpp"
#include "Core/TemplateEditor/TemplatesCommand.hpp"

#include <optional>

struct Templates;
struct DBTemplatesTab : IDBEditorTab {
  explicit DBTemplatesTab(Templates& templates, DatabaseEditor* parent)
  : IDBEditorTab(parent), m_templates(&templates) {
    if (m_templates->templates.size() > 0) {
      m_templateName = m_templates->templates.at(m_selection).name;
      m_templateType = static_cast<int>(m_templates->templates.at(m_selection).type);
      m_currentTemplate =
          CreateTemplateDialog(static_cast<Template::TemplateType>(m_templates->templates.at(m_selection).type));
    }
  }

  void draw() override;
  std::vector<Templates*>& templates() { return m_template; }

private:
  Templates* m_templates = nullptr;
  std::vector<Templates*> m_template;
  int m_selection{0};
  int m_templateType{0};
  std::string m_templateName;
  bool m_hasChanges = false;

  std::shared_ptr<IDBTemplates> m_currentTemplate;

  std::shared_ptr<IDBTemplates> CreateTemplateDialog(Template::TemplateType type) {
    switch (type) {
    case Template::TemplateType::Command:
      return std::make_shared<TemplatesCommand>(&m_templates->templates.at(m_selection), m_parent);
    case Template::TemplateType::Tint:
      return std::make_shared<TemplatesTint>(&m_templates->templates.at(m_selection), m_parent);
    default:
      break;
    }
  }
  void SetTemplate() {
    m_templateName = m_templates->templates.at(m_selection).name;
    m_templateType = static_cast<int>(m_templates->templates.at(m_selection).type);
    m_currentTemplate = CreateTemplateDialog(static_cast<Template::TemplateType>(m_templateType));
    m_hasChanges = false;
  }

  void SaveChanges() {
    m_templates->templates.at(m_selection).name = m_templateName;
    m_templates->templates.at(m_selection).type = static_cast<Template::TemplateType>(m_templateType);
    if (m_templates->templates.at(m_selection).type == Template::TemplateType::Command) {

    } else if (m_templates->templates.at(m_selection).type == Template::TemplateType::Tint) {
      m_templates->templates.at(m_selection).commands.clear();
      m_templates->templates.at(m_selection).commands = m_currentTemplate->getData();
    }
    // m_templates->templates.at(m_selection).parameters = 0; TODO
    // m_templates->templates.at(m_selection).commands = 0; TODO
  }
};
