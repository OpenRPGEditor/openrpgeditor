#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/IconSheet.hpp"
#include "Database/Armors.hpp"
#include "DBCommonEventsTab.hpp"

struct DBArmorsTab : IDBEditorTab {
  DBArmorsTab() = delete;
  explicit DBArmorsTab(DatabaseEditor* parent);
  void draw() override;

  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(const int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(const int start, const int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  std::string getName(const int index) override { return m_armors->armor(index)->name(); }
  int getCount() override { return m_armors->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Armors"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBArmorsTab"sv; };

  bool isReady() const override { return Database::instance()->armors && Database::instance()->system; }
  void initialize() override {
    if (!isReady()) {
      return;
    }

    m_armors = &Database::instance()->armors.value();
    m_selectedArmor = m_armors->armor(1);
    if (m_selectedArmor) {
      m_traitsEditor.setTraits(&m_selectedArmor->traits());
    }
  }

  bool isInitialized() const override { return m_armors != nullptr; }

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  Armors* m_armors = nullptr;
  Armor* m_selectedArmor{};
  int m_editMaxArmors{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  IconSheet* m_currentSheet = nullptr;
  std::optional<CheckeredCompositeTexture> m_itemButtonTexture;
  std::optional<IconSheet> m_itemSheet;
  std::optional<IconSheet> m_itemPicker;
  TraitsEditor m_traitsEditor;
};