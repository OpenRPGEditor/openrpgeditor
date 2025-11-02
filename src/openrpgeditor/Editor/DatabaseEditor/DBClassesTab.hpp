#pragma once
#include "DBCommonEventsTab.hpp"
#include "Editor/CommonUI/TraitsEditor.hpp"
#include "Editor/DatabaseEditor/IDBEditorTab.hpp"

#include "Database/Classes.hpp"

#include <array>

class DBClassesTab final : public IDBCoreEditorTab<DBClassesTab> {
public:
  enum class ExperienceGraphMode {
    Total,
    Next,
  };

  void draw() override;

  std::string getName(const int index) const override { return m_classes->classType(index)->name(); }
  int getCount() const override { return m_classes->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Classes"); }

  [[nodiscard]] bool isReady() const override { return Database::instance()->system && Database::instance()->skills && Database::instance()->states && Database::instance()->classes; }
  void initialize() override {
    if (!isReady()) {
      return;
    }
    m_classes = &Database::instance()->classes.value();

    m_selectedClass = m_classes->classType(1);
    if (m_selectedClass) {
      m_traitsEditor.setTraits(&m_selectedClass->traits());
    }
  }
  [[nodiscard]] bool isInitialized() const override { return m_classes != nullptr; }

private:
  void drawExperienceGraph(ExperienceGraphMode mode) const;
  void drawExpPopup();
  Classes* m_classes = nullptr;
  Class* m_selectedClass{};
  int m_editMaxClasses{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  bool m_showEquipEdit = false;
  TraitsEditor m_traitsEditor;
  std::array<int, 4> m_expWorkValues{};
};