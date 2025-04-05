#pragma once
#include "Core/TemplateEditor/IDBTemplates.hpp"
#include "Database/Templates.hpp"

class TemplatesTint final : public IDBTemplates {
public:
  TemplatesTint() = delete;
  explicit TemplatesTint(Template* templ, DatabaseEditor* parent)
  : IDBTemplates(parent)
  , m_currentTemplate(templ) {
    if (!m_currentTemplate->parameters().empty() && m_currentTemplate->parameters().size() == 4) {
      r = m_currentTemplate->parameters().at(0);
      g = m_currentTemplate->parameters().at(1);
      b = m_currentTemplate->parameters().at(2);
      gray = m_currentTemplate->parameters().at(3);
    }
  }
  void draw() override;
  DatabaseEditor* m_parent = nullptr;
  std::vector<int> getData() override { return std::vector({r, g, b, gray}); }
  bool hasChanges() override {
    return r != m_currentTemplate->parameters().at(0) || g != m_currentTemplate->parameters().at(1) || b != m_currentTemplate->parameters().at(2) || gray != m_currentTemplate->parameters().at(3);
  }

private:
  Template* m_currentTemplate;

  int r = 255;
  int g = 255;
  int b = 255;
  int gray = 255;
};