#pragma once
#include "Core/CommonUI/Directory.hpp"
#include "Core/Log.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Settings.hpp"
#include "Core/Sound.hpp"
#include "Database/Database.hpp"
#include "Database/Globals.hpp"
#include <iostream>

namespace fs = std::filesystem;
struct AudioEditor {
  AudioEditor() = delete;
  explicit AudioEditor(EventCode code, Audio& audio)
  : m_audio(audio) {
    if (code == EventCode::Play_SE) {
      m_audioDir.emplace("audio/se", ".ogg", m_audio.name());
    } else if (code == EventCode::Play_BGM || code == EventCode::Change_Battle_BGM || code == EventCode::Change_Vechicle_BGM) {
      m_audioDir.emplace("audio/bgm", ".ogg", m_audio.name());
    } else if (code == EventCode::Change_Defeat_ME || code == EventCode::Change_Victory_ME || code == EventCode::Play_ME) {
      m_audioDir.emplace("audio/me", ".ogg", m_audio.name());
    } else if (code == EventCode::Play_BGS) {
      m_audioDir.emplace("audio/bgs", ".ogg", m_audio.name());
    } else {
      APP_INFO("Audio code not implemented");
    }
    m_audios = m_audioDir.value().getDirectoryContents();
    m_folders = m_audioDir.value().getDirectories();
    std::string audioName = m_audioDir->getFileName(m_audio.name());
    for (int i = 0; i < m_audios.size(); ++i) {
      if (audioName == m_audios[i]) {
        m_selected = i;
      }
    }
  }
  bool draw();

private:
  bool m_confirmed{false};

  int m_selected = 0;
  Audio& m_audio;

  Sound m_sound;

  std::tuple<bool, bool> result;
  std::vector<std::string> m_audios;

  int m_selectedFolder{-1};
  std::optional<Directory> m_audioDir;
  std::vector<std::string> m_folders;

  bool playAudio(const std::string& path) {
    // Load and play music
    m_sound = Sound(ResourceManager::instance()->loadBGM(path));

    setVolume(m_audio.volume());
    setPanning(m_audio.pan());
    setPitch(m_audio.pitch());
    m_sound.play();
    return true;
  }
  void setVolume(int volume) {
    m_sound.setVolume(static_cast<float>(volume)); // 0% to 100%
  }
  void setPanning(int value) { m_sound.setPan(static_cast<float>(value) / 100.0f); }
  void setPitch(int value) { m_sound.setPitch(value / 100.f); }
  void stopAudio() { m_sound.stop(); }
};
