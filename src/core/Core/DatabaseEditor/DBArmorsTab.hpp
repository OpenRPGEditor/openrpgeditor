#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Armors;
struct DBArmorsTab : IDBEditorTab {
  DBArmorsTab() = delete;
  explicit DBArmorsTab(Armors& armors, DatabaseEditor* parent) : IDBEditorTab(parent), m_armors(armors) {}
  void draw() override;

private:
  Armors& m_armors;
};