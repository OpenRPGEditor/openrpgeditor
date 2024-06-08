#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Items;
struct DBItemsTab : IDBEditorTab {
  DBItemsTab() = delete;
  explicit DBItemsTab(Items& items) : m_items(items) {}
  void draw() override;

private:
  Items& m_items;
};