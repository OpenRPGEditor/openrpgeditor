#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

#include "Database/Classes.hpp"

#include <array>

struct DBClassesTab : IDBEditorTab {
  enum class ExperienceGraphMode {
    Total,
    Next,
  };

  DBClassesTab() = delete;
  explicit DBClassesTab(Classes& classes, DatabaseEditor* parent);
  void draw() override;

  Classes& classes() { return m_classes; }
  const Classes& classes() const { return m_classes; }

  Class* classType(int id) { return m_classes.classType(id); }
  const Class* classType(int id) const { return m_classes.classType(id); }

private:
  void drawExperienceGraph(ExperienceGraphMode mode) const;
  void drawExpPopup();
  Classes& m_classes;
  Class* m_selectedClass{};
  int m_editMaxClasses{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  bool m_showEquipEdit = false;
  TraitsEditor m_traitsEditor;
  std::array<int, 4> m_expWorkValues;
};