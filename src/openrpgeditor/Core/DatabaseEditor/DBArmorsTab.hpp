#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/IconSheet.hpp"
#include "DBCommonEventsTab.hpp"
#include "Database/Armors.hpp"

struct DBArmorsTab : IDBEditorTab {
  DBArmorsTab() = delete;
  explicit DBArmorsTab(Armors& armors, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Armor>& armors() { return m_armors.armors(); }
  [[nodiscard]] const std::vector<Armor>& armors() const { return m_armors.armors(); }
  Armor* armor(int id) { return m_armors.armor(id); }
  const Armor* armor(int id) const { return m_armors.armor(id); }
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  std::string getName(int index) override { return m_armors.armor(index)->name(); }
  int getCount() override { return m_armors.count(); }

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  Armors& m_armors;
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