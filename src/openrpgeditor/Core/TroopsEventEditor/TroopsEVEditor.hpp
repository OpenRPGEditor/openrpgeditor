#pragma once

#include "Core/TroopsEventEditor/TroopsEVEditor.hpp"
#include "Core/TroopsEventEditor/TroopsEVPage.hpp"
#include "Database/Troops.hpp"

#include <vector>

struct MainWindow;

struct TroopsEVEditor {
  TroopsEVEditor(MainWindow* parent, Troop* troop) : m_parent(parent), m_troop(troop) {
    for (auto& page : troop->m_pages) {
      m_pages.emplace_back(this, &page);
    }
    m_id = troop->id();
  }
  void fixupPages();

  bool draw();

  Troop* event() { return m_troop; }
  const Troop* event() const { return m_troop; }

  void setTroop(Troop* troop) {
    m_troop = troop;
    fixupPages();
  }

  MainWindow* project() { return m_parent; }
  const MainWindow* project() const { return m_parent; }

  int id() const { return m_id; }

private:
  MainWindow* m_parent = nullptr;
  Troop* m_troop = nullptr;
  int m_id;
  std::vector<TroopsEVPage> m_pages;
  int m_selectedPage = 0;
  bool m_isOpen = true;
  bool erased{false};
  int erasedIdx{-1};
};
