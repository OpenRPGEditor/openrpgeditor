#pragma once
#include "Core/CommonUI/ColorFlashPicker.hpp"
#include "Core/CommonUI/ImagePicker.hpp"
#include "Core/CommonUI/SoundPicker.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Animations.hpp"
#include "DBCommonEventsTab.hpp"

class DatabaseEditor;
class Animations;
class DBAnimationsTab final : public IDBEditorTab {
public:
  DBAnimationsTab() = delete;
  explicit DBAnimationsTab(DatabaseEditor* parent);
  void draw() override;

  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(const int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(const int start, const int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  std::string getName(const int index) override { return m_animations->animation(index)->name(); }
  int getCount() override { return m_animations->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Animations"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBAnimationsTab"sv; };
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
  }

  bool isInitialized() const override { return m_animations; }

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
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
  void resetSelection(Animation::Timing& timing);

  Animation::Timing getTiming(int frame, Audio audio, Animation::Color color, FlashScope scope, int duration);
};