#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/ChangeDefeatME.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

namespace fs = std::filesystem;
struct Dialog_ChangeDefeatME : IEventDialogController {
  Dialog_ChangeDefeatME() = delete;
  explicit Dialog_ChangeDefeatME(const std::string& name,
                                 const std::shared_ptr<ChangeDefeatMECommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeDefeatMECommand());
    }
    m_audio = command->me;
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
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};

  int m_selected = 0;
  Audio m_audio;

  sf::SoundBuffer buffer;
  sf::Sound sound;
  std::shared_ptr<ChangeDefeatMECommand> command;
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
  }
  void setPitch(int value) { sound.setPitch(value / 100.f); }
  void stopAudio() { sound.stop(); }
};
