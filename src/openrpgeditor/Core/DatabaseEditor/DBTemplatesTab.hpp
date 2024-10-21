#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Templates.hpp"

#include <optional>

struct Templates;
struct DBTemplatesTab : IDBEditorTab {
  explicit DBTemplatesTab(Templates& templates, DatabaseEditor* parent)
  : IDBEditorTab(parent), m_templates(&templates) {}

  void draw() override;

private:
  Templates* m_templates = nullptr;

  int m_selection = -1;
  std::optional<std::string> m_currentTemplate;
  bool m_openPopup = false;
};
