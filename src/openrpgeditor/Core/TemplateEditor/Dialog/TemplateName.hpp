#pragma once
#include "Core/TemplateEditor/IDBTemplates.hpp"
#include "Database/Templates.hpp"

class TemplateName : public IDBTemplates {
public:
  TemplateName() = delete;
  explicit TemplateName(Template* templ, DatabaseEditor* parent)
  : IDBTemplates(parent)
  , m_template(templ) {
    m_templateName = m_template->name();
  }
  bool hasChanges() override { return m_hasChanges; }
  void draw() override;
  DatabaseEditor* m_parent = nullptr;

private:
  Template* m_template;
  std::string m_templateName;

  bool m_hasChanges{false};
  bool m_open{true};
};