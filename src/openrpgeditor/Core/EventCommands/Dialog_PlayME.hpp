#pragma once
#include "Core/CommonUI/Directory.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Settings.hpp"
#include "Core/Sound.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/PlayME.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

namespace fs = std::filesystem;
struct Dialog_PlayME : IEventDialogController {
  Dialog_PlayME() = delete;
  explicit Dialog_PlayME(const std::string& name, const std::shared_ptr<PlayMECommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new PlayMECommand());
    }
    m_audio = command->audio;
    m_audioDir.emplace("audio/me", ".ogg", m_audio.name());
    m_audios = m_audioDir.value().getDirectoryContents();
    m_folders = m_audioDir.value().getDirectories();
    std::string audioName = m_audioDir->getFileName(m_audio.name());
    for (int i = 0; i < m_audios.size(); ++i) {
      if (audioName == m_audios[i]) {
        m_selected = i;
      }
    }
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};

  int m_selected = 0;
  Audio m_audio;

  Sound m_sound;

  std::shared_ptr<PlayMECommand> command;
  std::tuple<bool, bool> result;
  std::vector<std::string> m_audios;

  int m_selectedFolder{-1};
  std::optional<Directory> m_audioDir;
  std::vector<std::string> m_folders;

  bool playAudio(const std::string& path) {
    // Load and play music
    m_sound = Sound(ResourceManager::instance()->loadME(path));
    m_sound.play();
    setVolume(m_audio.volume());
    setPanning(m_audio.pan());
    setPitch(m_audio.pitch());
    return true;
  }
  void setVolume(int volume) {
    m_sound.setVolume(static_cast<float>(volume)); // 0% to 100%
  }
  void setPanning(int value) { m_sound.setPan(static_cast<float>(value) / 100.0f); }
  void setPitch(int value) { m_sound.setPitch(value / 100.f); }
  void stopAudio() { m_sound.stop(); }
};
