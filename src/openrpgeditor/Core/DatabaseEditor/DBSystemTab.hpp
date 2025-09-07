#pragma once
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/CommonUI/ImagePicker.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/GameWindowBackground.hpp"
#include "DBCommonEventsTab.hpp"

#include "Database/Actor.hpp"
#include "Database/System.hpp"

/* TODO: Move this somewhere else */
enum class AudioType {
  BackgroundMusic,
  Melody,
  SoundEffect,
};

struct System;
struct DBSystemTab : IDBEditorTab {
  DBSystemTab() = delete;
  explicit DBSystemTab(System& system, DatabaseEditor* parent);
  void draw() override;
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  std::string getName(int index) override { return ""; }
  int getCount() override { return 0; }

  [[nodiscard]] std::string tabName() const override { return tr("System"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBSystemTab"sv; };

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
  void addAudioRow(const Audio& audio, const std::string& type, AudioType audioType);

  System& m_system;
  std::optional<CharacterSheet> m_boatSheet;
  std::optional<CharacterSheet> m_airshipSheet;
  std::optional<CharacterSheet> m_shipSheet;
  CharacterSheet* m_currentSheet = nullptr;
  std::optional<CharacterPicker> m_characterPicker;
  std::optional<GameWindowBackground> m_gameWindowBackground = GameWindowBackground(192, 192);
  std::optional<CheckeredCompositeTexture> m_boatButtonTexture;
  std::optional<CheckeredCompositeTexture> m_shipButtonTexture;
  std::optional<CheckeredCompositeTexture> m_airshipButtonTexture;
  std::optional<ActorPicker> m_actorsPicker;
  std::optional<VariableSwitchPicker> m_skillTypePicker;
  std::optional<ImagePicker> m_titleImagePicker;
  int m_selectedActor = -1;
  int m_selectedSkill = -1;
  int m_selectedMusic = -1;
  int m_selectedSound = -1;
  int m_selectedMotion = -1;
};
