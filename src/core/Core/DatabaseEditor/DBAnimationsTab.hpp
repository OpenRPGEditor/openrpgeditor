#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct Animations;
struct DBAnimationsTab : IDBEditorTab {
  DBAnimationsTab() = delete;
  explicit DBAnimationsTab(Animations& system) : m_animations(system) {}
  void draw() override;

private:
  Animations& m_animations;
};