#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Animations.hpp"

struct DatabaseEditor;
struct Animations;
struct DBAnimationsTab : IDBEditorTab {
  DBAnimationsTab() = delete;
  explicit DBAnimationsTab(Animations& system, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Animation>& animations() { return m_animations.animations(); }
  [[nodiscard]] const std::vector<Animation>& animations() const { return m_animations.animations(); }
  [[nodiscard]] Animation* animation(int id) { return m_animations.animation(id); }
  [[nodiscard]] const Animation* animation(int id) const { return m_animations.animation(id); }

private:
  Animations& m_animations;
  Animation* m_selectedAnimation{};
  int m_editMaxAnimations{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
};