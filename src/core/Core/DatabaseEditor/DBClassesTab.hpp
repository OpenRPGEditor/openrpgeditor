#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Classes;
struct Class;
struct DBClassesTab : IDBEditorTab {
  DBClassesTab() = delete;
  explicit DBClassesTab(Classes& classes, DatabaseEditor* parent);
  void draw() override;

  Classes& classes() { return m_classes; }
  const Classes& classes() const { return m_classes; }

private:
  Classes& m_classes;
  Class* m_selectedClass{};
  int m_maxClasses{};
  int m_editMaxClasses;
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  bool m_showEquipEdit = false;
};