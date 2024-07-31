#pragma once
#include "Database/EventCommands/ChangeBattleBGM.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

struct Dialog_ChangeBattleBGM : IEventDialogController {
  Dialog_ChangeBattleBGM() = delete;
  explicit Dialog_ChangeBattleBGM(const std::string& name,
                                  const std::shared_ptr<ChangeBattleBGMCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeBattleBGMCommand());
    }
    m_audio = command->bgm;
    try {
      auto files = ResourceManager::instance()->getDirectoryContents("audio/bgm/", ".ogg");
      for (const auto& file : files) {
        m_audios.push_back(file);
      }
    } catch (const std::filesystem::filesystem_error& e) {
      std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    m_audio.name = m_audios.at(m_selected);
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};

  int m_selected = 0;
  Audio m_audio;

  sf::Sound sound;

  std::shared_ptr<ChangeBattleBGMCommand> command;
  std::tuple<bool, bool> result;
  std::vector<std::string> m_audios;

  bool playAudio(const std::string& path) {
    sound = sf::Sound(ResourceManager::instance()->loadBGM(path));
    sound.play();
    setVolume(m_audio.volume);
    setPanning(m_audio.pan);
    setPitch(m_audio.pitch);
    return true;
  }
  void setVolume(int volume) {
    sound.setVolume(volume); // 0% to 100%
  }
  void setPanning(int value) {
    // TODO -- how can we pan this thing
    /*
    *(-1,0,0)' is to the left of the listener
    (1,0,0)' is to the right of the listener
    (0,0,1)' is in front of the listener
    (0,0,-1)' is behind the listener
    */
    if (value > 0) {
      // To the right (+)
      // sound.setPosition(sf::Vector3f{ static_cast<float>(value), 0.f, 0.f});
      sound.setPosition(sf::Vector3f(static_cast<float>(value) / 100.f, 0, 0));
    } else { // To the left (-)
      sound.setPosition(sf::Vector3f(static_cast<float>(value) / 100.f, 0, 0));
    }
  }
  void setPitch(int value) { sound.setPitch(value / 100.f); }
  void stopAudio() { sound.stop(); }
};
