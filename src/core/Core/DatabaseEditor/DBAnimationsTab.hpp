#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"

struct DatabaseEditor;
struct Animations;
struct DBAnimationsTab : IDBEditorTab {
  DBAnimationsTab() = delete;
  explicit DBAnimationsTab(Animations& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_animations(system) {}
  void draw() override;

private:
  Animations& m_animations;
};