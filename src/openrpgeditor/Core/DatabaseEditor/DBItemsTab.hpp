#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Items.hpp"

struct Items;
struct DBItemsTab : IDBEditorTab {
  DBItemsTab() = delete;
  explicit DBItemsTab(Items& items, DatabaseEditor* parent) : IDBEditorTab(parent), m_items(items) {}
  void draw() override;

  [[nodiscard]] std::vector<Item>& items() { return m_items.items(); }
  [[nodiscard]] const std::vector<Item>& items() const { return m_items.items(); }

  [[nodiscard]] Item* item(int id) { return m_items.item(id); }
  [[nodiscard]] const Item* item(int id) const { return m_items.item(id); }

private:
  Items& m_items;
};