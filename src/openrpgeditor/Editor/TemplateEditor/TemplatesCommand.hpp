#pragma once
#include "Database/Templates.hpp"
#include "Editor/TemplateEditor/IDBTemplates.hpp"

class TemplatesCommand final : public IDBTemplates {
public:
  TemplatesCommand() = delete;
  explicit TemplatesCommand(Template* templ);
  void draw() override;
  DatabaseEditor* m_parent = nullptr;

private:
  Template* m_currentTemplate;
};