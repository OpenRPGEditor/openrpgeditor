#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Items;
struct DBItemsTab : IDBEditorTab {
  DBItemsTab() = delete;
  explicit DBItemsTab(Items& items, DatabaseEditor* parent) : IDBEditorTab(parent), m_items(items) {}
  void draw() override;

private:
  Items& m_items;
};