#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Settings.hpp"

#include "Core/Sound.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/ChangeBattleBGM.hpp"

#include <iostream>

namespace fs = std::filesystem;
struct Dialog_ChangeBattleBGM : IEventDialogController {
  Dialog_ChangeBattleBGM() = delete;
  explicit Dialog_ChangeBattleBGM(const std::string& name, const std::shared_ptr<ChangeBattleBGMCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeBattleBGMCommand());
    }
    m_audio = command->bgm;
    try {
      m_audios = ResourceManager::instance()->getDirectoryContents("audio/bgm/", ".ogg");
    } catch (const std::filesystem::filesystem_error& e) { std::cerr << "Error accessing directory: " << e.what() << std::endl; }
    m_audio.name.clear();
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};

  int m_selected = 0;
  Audio m_audio;

  Sound m_sound;

  std::shared_ptr<ChangeBattleBGMCommand> command;
  std::tuple<bool, bool> result;
  std::vector<std::string> m_audios;

  bool playAudio(const std::string& path) {
    // Load and play music
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
