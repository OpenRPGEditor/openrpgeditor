#pragma once
#include "Core/CommonUI/ColorFlashPicker.hpp"
#include "Core/CommonUI/ImagePicker.hpp"
#include "Core/CommonUI/SoundPicker.hpp"
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
  int m_selectedTiming{0};

  bool m_isApplyingChanges{false};

  std::vector<Animation::Timing> m_selectedTimings;

  int m_duration{5};
  Audio m_selectedAudio;
  Animation::Color m_selectedColor;
  int m_selectedScope;
  int m_selectedDuration;
  int m_selectedFrameNumber;

  std::optional<ImagePicker> m_imagePicker;
  ColorFlashPicker m_colorPicker;
  int m_pickerSelection;

  SoundPicker m_soundPicker;

  void onNameModified(Audio*, const std::string_view name) { m_selectedAudio.setName(name.data()); }
  void onVolModified(Audio*, const int volume) { m_selectedAudio.setVolume(volume); }
  void onPanModified(Audio*, const int pan) { m_selectedAudio.setPan(pan); }
  void onPitchModified(Audio*, const int pitch) { m_selectedAudio.setPitch(pitch); }

  Animation::Timing getTiming(int frame, Audio audio, Animation::Color color, FlashScope scope, int duration);
};