#pragma once
#include "Database/Armors.hpp"
#include "DBCommonEventsTab.hpp"
#include "Editor/CommonUI/TraitsEditor.hpp"
#include "Editor/DatabaseEditor/IDBEditorTab.hpp"
#include "Editor/Graphics/CheckeredCompositeTexture.hpp"
#include "Editor/Graphics/IconSheet.hpp"

struct DBArmorsTab final : public IDBCoreEditorTab<DBArmorsTab> {
  void draw() override;

  std::string getName(const int index) const override { return m_armors->armor(index)->name(); }
  int getCount() const override { return m_armors->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Armors"); }

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