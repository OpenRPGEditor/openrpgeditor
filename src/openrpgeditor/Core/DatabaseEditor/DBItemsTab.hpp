#pragma once
#include "Core/CommonUI/EffectsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/IconSheet.hpp"
#include "Database/Animation.hpp"
#include "Database/Items.hpp"
#include "DBCommonEventsTab.hpp"

class Items;
class DBItemsTab final : public IDBEditorTab {
public:
  DBItemsTab() = delete;
  explicit DBItemsTab(DatabaseEditor* parent);
  void draw() override;

  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(const int index) override { return m_headers.at(index); }
  void setHeaderRange(const int start, const int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  bool hasHeader() override { return !m_headers.empty(); }

  std::string getName(int index) override { return m_items->item(index)->name(); }
  int getCount() override { return m_items->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Items"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBItemsTab"sv; };

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
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
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