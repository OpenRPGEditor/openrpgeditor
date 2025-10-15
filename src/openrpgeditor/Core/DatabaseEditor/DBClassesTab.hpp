#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "DBCommonEventsTab.hpp"

#include "Database/Classes.hpp"

#include <array>

struct DBClassesTab final : IDBEditorTab {
  enum class ExperienceGraphMode {
    Total,
    Next,
  };

  DBClassesTab() = delete;
  explicit DBClassesTab(DatabaseEditor* parent);
  void draw() override;

  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(const int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(const int start, const int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  std::string getName(const int index) override { return m_classes->classType(index)->name(); }
  int getCount() override { return m_classes->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Classes"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBClassesTab"sv; };

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
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
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
  std::array<int, 4> m_expWorkValues;
};