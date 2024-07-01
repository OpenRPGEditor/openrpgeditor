#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Tilesets.hpp"

struct DBTilesetsTab : IDBEditorTab {
  DBTilesetsTab() = delete;
  explicit DBTilesetsTab(Tilesets& tilesets, DatabaseEditor* parent) : IDBEditorTab(parent), m_tilesets(tilesets) {}
  void draw() override;

  [[nodiscard]] std::vector<Tileset>& tilesets() { return m_tilesets.tilesets(); }
  [[nodiscard]] const std::vector<Tileset>& tilesets() const { return m_tilesets.tilesets(); }

  [[nodiscard]] Tileset* tileset(int id) { return m_tilesets.tileset(id); }
  [[nodiscard]] const Tileset* tileset(int id) const { return m_tilesets.tileset(id); }

private:
  Tilesets& m_tilesets;
};