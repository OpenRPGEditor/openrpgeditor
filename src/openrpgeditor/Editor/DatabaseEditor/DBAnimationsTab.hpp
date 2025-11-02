#pragma once
#include "Database/Animations.hpp"
#include "DBCommonEventsTab.hpp"
#include "Editor/CommonUI/ColorFlashPicker.hpp"
#include "Editor/CommonUI/ImagePicker.hpp"
#include "Editor/CommonUI/SoundPicker.hpp"
#include "Editor/DatabaseEditor/IDBEditorTab.hpp"

class DatabaseEditor;
class Animations;
class DBAnimationsTab final : public IDBCoreEditorTab<DBAnimationsTab> {
public:
  void draw() override;

  std::string getName(const int index) const override { return m_animations->animation(index)->name(); }
  int getCount() const override { return m_animations->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Animations"); }
  bool isExperimental() const override { return true; }

  [[nodiscard]] bool isReady() const override { return !!Database::instance()->animations; }
  void initialize() override {
    if (!isReady()) {
      return;
    }
    m_animations = &Database::instance()->animations.value();

    m_selectedAnimation = m_animations->animation(1);

    if (m_selectedAnimation) {
      m_selectedTimings = m_selectedAnimation->timings();
    }

    m_colorPicker.setValues(255, 255, 255, 255);
    m_selectedAudio.setName("");
    m_selectedAudio.setVolume(100);
    m_selectedAudio.setPan(0);
    m_selectedAudio.setPitch(100);

    if (m_selectedAnimation) {
      m_selectedAnimation->setTimeLine(std::vector(m_selectedAnimation->frames().size(), false));
    }

    m_soundPicker.selectedAudio().nameModified().connect<&DBAnimationsTab::onNameModified>(this);
    m_soundPicker.selectedAudio().volumeModified().connect<&DBAnimationsTab::onVolModified>(this);
    m_soundPicker.selectedAudio().panModified().connect<&DBAnimationsTab::onPanModified>(this);
    m_soundPicker.selectedAudio().pitchModified().connect<&DBAnimationsTab::onPitchModified>(this);
  }

  bool isInitialized() const override { return m_animations; }

private:
  Animations* m_animations = nullptr;
  Animation* m_selectedAnimation{};
  int m_editMaxAnimations{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;

  int m_frameCursor{0};
  int m_timingSE{0};
  int m_frameType{0};
  int m_selectedTiming{0};

  bool m_isApplyingChanges{false};

  std::vector<Animation::Timing> m_selectedTimings;

  int m_duration{5};
  Audio m_selectedAudio;
  Animation::Color m_selectedColor;
  int m_selectedScope{};
  int m_selectedDuration{};
  int m_selectedFrameNumber{};

  std::optional<ImagePicker> m_imagePicker;
  ColorFlashPicker m_colorPicker;
  int m_pickerSelection{};
  SoundPicker m_soundPicker{};

  void onNameModified(Audio*, const std::string_view name) { m_selectedAudio.setName(name.data()); }
  void onVolModified(Audio*, const int volume) { m_selectedAudio.setVolume(volume); }
  void onPanModified(Audio*, const int pan) { m_selectedAudio.setPan(pan); }
  void onPitchModified(Audio*, const int pitch) { m_selectedAudio.setPitch(pitch); }
  void resetSelection(Animation::Timing& timing);

  Animation::Timing getTiming(int frame, Audio audio, Animation::Color color, FlashScope scope, int duration);
};