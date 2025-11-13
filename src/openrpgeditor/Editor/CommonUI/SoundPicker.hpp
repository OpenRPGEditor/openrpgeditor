#pragma once
#include "Database/Audio.hpp"
#include "Database/Database.hpp"
#include "Directory.hpp"

#include <iostream>
#include <SFML/Audio.hpp>

/**
 * @name SoundPicker
 * Provides a common UI tool to select a sound file
 */
namespace fs = std::filesystem;
class SoundPicker {
public:
  SoundPicker() = default;

  [[nodiscard]] Audio& selectedAudio() { return m_audio; }

  /**
   * Draws the SoundPicker elements.
   * @return True if any of the values were modified, false otherwise
   */
  bool draw();
  void show(SEType type) {
    m_audios = m_audioDir.value().getDirectoryContents();
    m_folders = m_audioDir.value().getDirectories();
    m_type = type;
    m_open = true;
    m_audioDir.emplace("audio/se", ".ogg", m_audio.name());
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

  int m_selectedFolder{-1};
  std::optional<Directory> m_audioDir;
  std::vector<std::string> m_folders;

  bool playAudio(const std::string& path) {
    // Load and play music
    if (!buffer.loadFromFile((Database::instance()->basePath / path ).replace_extension(m_audioDir->getExt()).generic_string())) {
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
