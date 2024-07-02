#pragma once
#include "IDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/Project.hpp"
#include "Core/Settings.hpp"
#include "Database/EventCommands/PlayBGM.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

namespace fs = std::filesystem;
struct Project;
struct Dialog_PlayBGM : IDialogController {
  Dialog_PlayBGM() = delete;
  explicit Dialog_PlayBGM(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    try {
    auto files = getFileNames(Database::Instance->basePath + "audio/bgm/" );
    for (const auto& file : files) {
      m_audios.push_back(file);
    }
  } catch (const std::filesystem::filesystem_error& e) {
    std::cerr << "Error accessing directory: " << e.what() << std::endl;
  }
    //setVolume(m_volume);
    //setPitch(m_pitch);
    //setPanning(m_pan);
    sf::Listener::setPosition(0.f, 0.f, 0.f);  // Set listener position
    APP_INFO(std::to_string(sound.getVolume()));
    APP_INFO(std::to_string(sound.getPitch()));
    APP_INFO(std::to_string(sound.getPosition().x) + " " + std::to_string(sound.getPosition().y));

    command.emplace();
  }
  std::tuple<bool, bool> draw() override;
  std::optional<PlayBGMCommand> getCommandData() { return command; }
  Project* m_project = nullptr;
private:
  bool m_confirmed{false};

  int m_selected = 0;
  int m_volume = 100;
  int m_pitch = 100;
  int m_pan = 0;

  sf::SoundBuffer buffer;
  sf::Sound sound;

  std::optional<PlayBGMCommand> command;
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
    sound.setRelativeToListener(false); // Ensure sound is not relative to listener
    sound.setBuffer(buffer);
    sound.play();
    return true;
  }
  void setVolume(int volume) {
    sound.setVolume(volume); // 0% to 100%
  }
  void setPanning(int value) {
    if (value > 0) { // To the right
      sound.setPosition(0.0f, static_cast<float>(value), 0.0f);
    }
    else { // To the left
      sound.setPosition(static_cast<float>(value), 0.0f, 0.0f);
    }
    APP_INFO(std::to_string(sound.getPosition().x) + " " + std::to_string(sound.getPosition().y));
  }
  bool setPitch(int value) {
    sound.setPitch(0.5f);
  }
  void stopAudio() {
    sound.stop();
  }
};
