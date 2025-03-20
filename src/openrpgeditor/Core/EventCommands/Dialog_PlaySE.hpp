#pragma once
#include "Core/CommonUI/Directory.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/Settings.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/PlaySE.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

namespace fs = std::filesystem;
struct Dialog_PlaySE : IEventDialogController {
  Dialog_PlaySE() = delete;
  explicit Dialog_PlaySE(const std::string& name, const std::shared_ptr<PlaySECommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new PlaySECommand());
    }
    m_audio = command->audio;
    m_audioDir.emplace("audio/se", ".ogg", m_audio.name());
    m_audios = m_audioDir.value().getDirectoryContents();
    m_folders = m_audioDir.value().getDirectories();

    std::string audioName = m_audioDir->getFileName(m_audio.name());
    for (int i = 0; i < m_audios.size(); ++i) {
      if (audioName == m_audios[i]) {
        m_selected = i + 1;
      }
    }
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};

  int m_selected = 0;
  Audio m_audio;

  sf::SoundBuffer buffer;
  sf::Sound sound;

  int m_selectedFolder{-1};
  std::optional<Directory> m_audioDir;
  std::vector<std::string> m_folders;

  std::shared_ptr<PlaySECommand> command;
  std::tuple<bool, bool> result;
  std::vector<std::string> m_audios;

  bool playAudio(const char* path) {
    // Load and play music
    APP_INFO(path);
    if (!buffer.loadFromFile(Database::instance()->basePath + path + m_audioDir.value().extFilter)) {
      // error loading file
      return false;
    }
    sf::Listener::setPosition(0.f, 0.f, 0.f); // Set listener position
    sound.setRelativeToListener(true);        // Ensure sound is not relative to listener
    sound.setBuffer(buffer);
    sound.play();
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
    APP_INFO("Listener: " + std::to_string(sf::Listener::getPosition().x) + " " + std::to_string(sf::Listener::getPosition().y));
    APP_INFO("Sound: " + std::to_string(sound.getPosition().x) + " " + std::to_string(sound.getPosition().y));
  }
  void setPitch(int value) { sound.setPitch(value / 100.f); }
  void stopAudio() { sound.stop(); }
};
