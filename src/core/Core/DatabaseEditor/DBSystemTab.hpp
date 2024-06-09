#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/System.hpp"

struct System;
struct DBSystemTab : IDBEditorTab {
  DBSystemTab() = delete;
  explicit DBSystemTab(System& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_system(system) {}
  void draw() override;

  const std::string variable(int id) const { return m_system.variables[id]; }
  const std::string switche(int id) const { return m_system.switches[id]; }
private:
  System& m_system;
};
