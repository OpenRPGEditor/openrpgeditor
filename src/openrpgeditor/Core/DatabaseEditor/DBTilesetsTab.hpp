#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Tilesets;
struct DBTilesetsTab : IDBEditorTab {
  DBTilesetsTab() = delete;
  explicit DBTilesetsTab(Tilesets& tilesets, DatabaseEditor* parent) : IDBEditorTab(parent), m_tilesets(tilesets) {}
  void draw() override;

private:
  Tilesets& m_tilesets;
};