#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Classes;
struct DBClassesTab : IDBEditorTab {
  DBClassesTab() = delete;
  explicit DBClassesTab(Classes& classes, DatabaseEditor* parent) : IDBEditorTab(parent), m_classes(classes) {}
  void draw() override;

  Classes& classes() { return m_classes; }
  const Classes& classes() const { return m_classes; }

private:
  Classes& m_classes;
};