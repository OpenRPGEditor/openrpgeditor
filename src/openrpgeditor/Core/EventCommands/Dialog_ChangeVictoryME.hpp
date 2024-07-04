#pragma once
#include "IDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/Project.hpp"
#include "Core/Settings.hpp"
#include "Database/EventCommands/ChangeVictoryME.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

namespace fs = std::filesystem;
struct Project;
struct Dialog_ChangeVictoryME : IDialogController {
  Dialog_ChangeVictoryME() = delete;
  explicit Dialog_ChangeVictoryME(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
    m_audio = Audio();
    try {
      auto files = getFileNames(Database::Instance->basePath + "audio/me/");
      for (const auto& file : files) {
        m_audios.push_back(file);
      }
    } catch (const std::filesystem::filesystem_error& e) {
      std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    m_audio.name = m_audios.at(m_selected);
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeVictoryMECommand>(command.value()); }

  Project* m_project = nullptr;

private:
  bool m_confirmed{false};

  int m_selected = 0;
  Audio m_audio;

  sf::SoundBuffer buffer;
  sf::Sound sound;

  std::optional<ChangeVictoryMECommand> command;
  std::tuple<bool, bool> result;
  std::vector<std::string> m_audios;
  std::vector<std::string> getFileNames(const std::string& directoryPath) {
    std::vector<std::string> fileNames;

    for (const auto& entry : fs::directory_iterator(directoryPath)) {
      std::string filename = entry.path().filename().string();
      size_t lastDotPos = filename.find_last_of(".");
      if (lastDotPos != std::string::npos) {
        std::string str = filename.substr(0, lastDotPos);
        fileNames.push_back(str);
      } else {
        fileNames.push_back(filename);
      }
    }
    return fileNames;
  }

  bool playAudio(const char* path) {
    // Load and play music
    APP_INFO(path);
    if (!buffer.loadFromFile(path)) {
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
    APP_INFO("Listener: " + std::to_string(sf::Listener::getPosition().x) + " " +
             std::to_string(sf::Listener::getPosition().y));
    APP_INFO("Sound: " + std::to_string(sound.getPosition().x) + " " + std::to_string(sound.getPosition().y));
  }
  void setPitch(int value) { sound.setPitch(value / 100.f); }
  void stopAudio() { sound.stop(); }
};
