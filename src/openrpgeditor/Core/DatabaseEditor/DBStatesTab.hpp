#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/States.hpp"
#include "DBCommonEventsTab.hpp"

struct DBStatesTab final : IDBEditorTab {
  void draw() override;

  std::string getName(const int index) const override { return m_states->state(index)->name(); }
  int getCount() const override { return m_states->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("States"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBStatesTab"sv; };

  bool isReady() const override { return !!Database::instance()->states; }
  bool isInitialized() const override { return m_states; }

  void initialize() override {
    if (!isReady()) {
      return;
    }
    m_states = &Database::instance()->states.value();
    m_selectedState = m_states->state(1);
    if (m_selectedState) {
      m_traitsEditor.setTraits(&m_selectedState->traits());
    }
  }

private:
  States* m_states{};
  State* m_selectedState{};
  int m_editMaxStates{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  TraitsEditor m_traitsEditor;
};