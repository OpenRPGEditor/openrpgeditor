#pragma once

#include "Database/Troops.hpp"
#include "Editor/TroopsEventEditor/TroopsEVEditor.hpp"
#include "Editor/TroopsEventEditor/TroopsEVPage.hpp"

#include <vector>

struct MainWindow;

struct TroopsEVEditor {
  explicit TroopsEVEditor(Troop* troop)
  : m_troop(troop) {
    if (m_troop) {
      for (auto& page : troop->pages()) {
        m_pages.emplace_back(this, &page, m_troop->id());
      }
      m_id = troop->id();
    }
  }
  void fixupPages();

  bool draw();

  Troop* event() { return m_troop; }
  const Troop* event() const { return m_troop; }

  void setTroop(Troop* troop) {
    m_troop = troop;
    fixupPages();
  }

  int id() const { return m_id; }

private:
  Troop* m_troop = nullptr;
  int m_id = 0;
  std::vector<TroopsEVPage> m_pages;
  int m_selectedPage = 0;
  bool m_isOpen = true;
  bool m_erased{false};
  int m_erasedIdx{-1};
};
