#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Classes;
struct DBClassesTab : IDBEditorTab {
  DBClassesTab() = delete;
  explicit DBClassesTab(Classes& classes) : m_classes(classes) {}
  void draw() override;

private:
  Classes& m_classes;
};