#pragma once
#include "Database/Audio.hpp"
#include "Database/Database.hpp"

#include <SFML/Audio.hpp>
#include <iostream>

/**
 * @name SoundPicker
 * Provides a common UI tool to select a sound file
 */
namespace fs = std::filesystem;
class SoundPicker {
public:
  SoundPicker() = default;

  [[nodiscard]] Audio selectedAudio() const { return m_audio; }

  /**
   * Draws the SoundPicker elements.
   * @return True if any of the values were modified, false otherwise
   */
  bool draw();
  void show(SEType type) {
    m_type = type;
    try {
      auto files = getFileNames(Database::instance()->basePath + getDirectoryString());

      for (const auto& file : files) {
        m_audios.push_back(file);
      }
    } catch (const std::filesystem::filesystem_error& e) { std::cerr << "Error accessing directory: " << e.what() << std::endl; }

    m_open = true;
  };

private:
  Audio m_audio;
  std::vector<std::string> m_audios;
  int m_selected{0};
  bool m_toneInvalid{true};
  bool m_modified{false};
  bool m_open{false};

  SEType m_type;

  sf::SoundBuffer buffer;
  sf::Sound sound;

  std::string getDirectoryString() {
    if (m_type == SEType::ME) {
      return "audio/me/";
    }
    if (m_type == SEType::SE) {
      return "audio/se/";
    }
    if (m_type == SEType::BGS) {
      return "audio/bgs/";
    }
    if (m_type == SEType::BGM) {
      return "audio/bgm/";
    }
  }
  std::vector<std::string> getFileNames(const std::string& directoryPath) {
    std::vector<std::string> fileNames;

    for (const auto& entry : fs::directory_iterator(directoryPath)) {
      if (entry.path().extension() != ".ogg")
        continue;

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
