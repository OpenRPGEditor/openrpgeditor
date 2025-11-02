#pragma once
#include "Database/Animation.hpp"
#include "Database/Items.hpp"
#include "DBCommonEventsTab.hpp"
#include "Editor/CommonUI/EffectsEditor.hpp"
#include "Editor/DatabaseEditor/IDBEditorTab.hpp"
#include "Editor/Graphics/CheckeredCompositeTexture.hpp"
#include "Editor/Graphics/IconSheet.hpp"

class Items;
class DBItemsTab final : public IDBCoreEditorTab<DBItemsTab> {
public:
  void draw() override;

  std::string getName(const int index) const override { return m_items->item(index)->name(); }
  int getCount() const override { return m_items->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Items"); }

  bool isReady() const override { return !!Database::instance()->items; }
  bool isInitialized() const override { return m_items; }

  void initialize() override {
    if (!isReady()) {
      return;
    }

    m_items = &Database::instance()->items.value();
    m_selectedItem = m_items->item(1);
    if (m_selectedItem) {
      m_effectsEditor.setEffects(&m_selectedItem->effects());
    }
  }

private:
  Items* m_items{};
  Item* m_selectedItem{};
  int m_editMaxItems{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  IconSheet* m_currentSheet = nullptr;
  std::optional<CheckeredCompositeTexture> m_itemButtonTexture;
  std::optional<IconSheet> m_itemSheet;
  std::optional<IconSheet> m_itemPicker;
  EffectsEditor m_effectsEditor;
  std::optional<AnimationPicker> m_animationPicker;
};