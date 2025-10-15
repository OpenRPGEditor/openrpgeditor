#pragma once
#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/CommonUI/Preview.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/CommonEvents.hpp"

struct DBCommonEventsTab : IDBEditorTab {
  explicit DBCommonEventsTab(DatabaseEditor* parent);
  ~DBCommonEventsTab() override {
    if (m_instance == this) {
      m_instance = nullptr;
    }
  }
  void draw() override;

  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(const int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(const int start, const int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  std::string getName(const int index) override { return m_events->event(index)->name(); }
  int getCount() override { return m_events->count(); }
  int getSelectedIndex() const { return m_selectedCommonEvent ? m_selectedCommonEvent->id() : 0; }

  [[nodiscard]] std::string tabName() const override { return tr("Common Events"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBCommonEventsTab"sv; };

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

  static DBCommonEventsTab* instance() { return m_instance; }

private:
  static DBCommonEventsTab* m_instance;
  CommonEvents* m_events = nullptr;
  CommonEvent* m_selectedCommonEvent{};
  EventCommandEditor m_commandEditor;
  std::optional<VariableSwitchPicker> picker;
  std::optional<Preview> preview;

  std::vector<int> m_headers;
  int m_categoryStart{0};
  int m_categoryEnd{0};

  int m_editMaxCommonEvents{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
};