#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Animations.hpp"

struct DatabaseEditor;
struct Animations;
struct DBAnimationsTab : IDBEditorTab {
  DBAnimationsTab() = delete;
  explicit DBAnimationsTab(Animations& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_animations(system) {}
  void draw() override;

  [[nodiscard]] std::vector<Animation>& animations() { return m_animations.animations(); }
  [[nodiscard]] const std::vector<Animation>& animations() const { return m_animations.animations(); }
  [[nodiscard]] Animation* animation(int id) { return m_animations.animation(id); }
  [[nodiscard]] const Animation* animation(int id) const { return m_animations.animation(id); }

private:
  Animations& m_animations;
};