#pragma once
#include "Core/TemplateEditor/IDBTemplates.hpp"
#include "Database/Templates.hpp"

struct TemplatesCommand : IDBTemplates {
  TemplatesCommand() = delete;
  explicit TemplatesCommand(Template* templ, DatabaseEditor* parent);
  void draw() override;
  DatabaseEditor* m_parent = nullptr;

private:
  Template* m_currentTemplate;
};