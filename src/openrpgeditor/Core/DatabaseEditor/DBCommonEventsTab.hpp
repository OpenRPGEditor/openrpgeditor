#pragma once
#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/CommonUI/Preview.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/CommonEvents.hpp"

class DBCommonEventsTab final : public IDBCoreEditorTab<DBCommonEventsTab> {
public:
  void draw() override;

  std::string getName(const int index) const override { return m_events->event(index)->name(); }
  int getCount() const override { return m_events->count(); }
  int getSelectedIndex() const { return m_selectedCommonEvent ? m_selectedCommonEvent->id() : 0; }

  [[nodiscard]] std::string tabName() const override { return tr("Common Events"); }

  [[nodiscard]] bool isReady() const override { return Database::instance()->system && Database::instance()->commonEvents; }

  void initialize() override {
    if (!isReady()) {
      return;
    }

    m_events = &Database::instance()->commonEvents.value();

    m_selectedCommonEvent = m_events->event(1);
    m_commandEditor.setCommands(&m_selectedCommonEvent->commands());

    for (const auto& cmn : m_events->events()) {
      if (cmn.has_value()) {
        if (hasUnicodeFormatting(cmn.value().name())) {
          m_headers.push_back(cmn.value().id());
        }
      }
    }
  }
  bool isInitialized() const override { return m_events; }

private:
  CommonEvents* m_events = nullptr;
  CommonEvent* m_selectedCommonEvent{};
  EventCommandEditor m_commandEditor;
  std::optional<VariableSwitchPicker> picker;
  std::optional<Preview> preview;

  int m_editMaxCommonEvents{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
};