#pragma once
#include "Core/CommonUI/EffectsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/IconSheet.hpp"
#include "DBCommonEventsTab.hpp"
#include "Database/Animation.hpp"
#include "Database/Items.hpp"

class Items;
class DBItemsTab final : public IDBEditorTab {
public:
  DBItemsTab() = delete;
  explicit DBItemsTab(Items& items, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Item>& items() { return m_items.items(); }
  [[nodiscard]] const std::vector<Item>& items() const { return m_items.items(); }

  [[nodiscard]] Item* item(const int id) { return m_items.item(id); }
  [[nodiscard]] const Item* item(const int id) const { return m_items.item(id); }
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  bool hasHeader() override { return !m_headers.empty(); }

  std::string getName(int index) override { return m_items.item(index)->name(); }
  int getCount() override { return m_items.count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Items"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBItemsTab"sv; };

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  Items& m_items;
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
  std::optional<ObjectPicker<Animation>> m_animationPicker;
};