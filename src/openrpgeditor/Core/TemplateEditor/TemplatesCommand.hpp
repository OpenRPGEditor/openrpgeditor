#pragma once
#include "Core/TemplateEditor/IDBTemplates.hpp"
#include "Database/Templates.hpp"

class TemplatesCommand final : public IDBTemplates {
public:
  TemplatesCommand() = delete;
  explicit TemplatesCommand(Template* templ);
  void draw() override;
  DatabaseEditor* m_parent = nullptr;

private:
  Template* m_currentTemplate;
};