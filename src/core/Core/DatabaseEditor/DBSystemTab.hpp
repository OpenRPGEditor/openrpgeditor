#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"



struct System;
struct DBSystemTab : IDBEditorTab {
  DBSystemTab() = delete;
  explicit DBSystemTab(System& system) : m_system(system) {}
  void draw() override;

private:
  System& m_system;
};
