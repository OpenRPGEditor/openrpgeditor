#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "DBCommonEventsTab.hpp"

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
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  std::string getName(int index) override { return m_classes.classType(index)->name(); }
  int getCount() override { return m_classes.count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Classes"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBClassesTab"sv; };

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
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