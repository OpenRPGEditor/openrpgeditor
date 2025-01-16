#pragma once
#include "Core/CommonUI/ImagePicker.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Animations.hpp"

class DatabaseEditor;
class Animations;
class DBAnimationsTab final : public IDBEditorTab {
public:
  DBAnimationsTab() = delete;
  explicit DBAnimationsTab(Animations& animations, DatabaseEditor* parent);
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

  int m_frameCursor{1};
  int m_timingSE{0};
  int m_frameType{0};

  int r{255};
  int g{255};
  int b{255};
  int m_intensity{255};
  int m_duration{5};

  std::optional<ImagePicker> m_imagePicker;
  int m_pickerSelection;
};