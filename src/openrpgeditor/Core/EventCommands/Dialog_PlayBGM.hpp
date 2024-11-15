#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/Settings.hpp"
#include "Core/ResourceManager.hpp"

#include "Database/Database.hpp"
#include "Database/EventCommands/PlayBGM.hpp"
#include "Core/Sound.hpp"

#include <iostream>

namespace fs = std::filesystem;
struct Dialog_PlayBGM : IEventDialogController {
  Dialog_PlayBGM() = delete;
  explicit Dialog_PlayBGM(const std::string& name, const std::shared_ptr<PlayBGMCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new PlayBGMCommand());
    }
    m_audio = command->audio;
    try {
      m_audios = ResourceManager::instance()->getDirectoryContents("audio/bgm/", ".ogg");
    } catch (const std::filesystem::filesystem_error& e) {
      std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    m_audio.name.clear();
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};

  int m_selected = 0;
  Audio m_audio;

  Sound m_sound;

  std::shared_ptr<PlayBGMCommand> command;
  std::tuple<bool, bool> result;
  std::vector<std::string> m_audios;

  bool playAudio(const std::string& path) {
    // Load and play music
    APP_INFO(path);
    m_sound = Sound(ResourceManager::instance()->loadBGM(path));
    m_sound.play();
    setVolume(m_audio.volume);
    setPanning(m_audio.pan);
    setPitch(m_audio.pitch);
    return true;
  }
  void setVolume(int volume) {
    m_sound.setVolume(static_cast<float>(volume)); // 0% to 100%
  }
  void setPanning(int value) { m_sound.setPan(static_cast<float>(value) / 100.0f); }
  void setPitch(int value) { m_sound.setPitch(value / 100.f); }
  void stopAudio() { m_sound.stop(); }
};
