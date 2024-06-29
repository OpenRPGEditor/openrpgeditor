#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Armors.hpp"

struct DBArmorsTab : IDBEditorTab {
  DBArmorsTab() = delete;
  explicit DBArmorsTab(Armors& armors, DatabaseEditor* parent) : IDBEditorTab(parent), m_armors(armors) {}
  void draw() override;

  [[nodiscard]] std::vector<Armor>& armors() { return m_armors.armors(); }
  [[nodiscard]] const std::vector<Armor>& armors() const { return m_armors.armors(); }
  Armor* armor(int id) { return m_armors.armor(id); }
  const Armor* armor(int id) const { return m_armors.armor(id); }

private:
  Armors& m_armors;
};