#pragma once
#include "Database/Database.hpp"
#include "Database/Globals.hpp"
#include "Editor/CommonUI/Directory.hpp"
#include "Editor/Log.hpp"
#include "Editor/ResourceManager.hpp"
#include "Editor/Settings.hpp"
#include "Editor/Sound.hpp"
#include <iostream>

namespace fs = std::filesystem;
struct AudioEditor {
  enum class AudioType {
    SE,
    BGM,
    ME,
    BGS,
    Vocals,
  };
  AudioEditor() = delete;
  explicit AudioEditor(const EventCode code, Audio& audio)
  : m_audio(audio) {
    if (code == EventCode::Play_SE) {
      m_audioDir.emplace("audio/se", ".ogg", m_audio.name());
      m_audioType = AudioType::SE;
    } else if (code == EventCode::Play_BGM || code == EventCode::Change_Battle_BGM || code == EventCode::Change_Vehicle_BGM) {
      m_audioDir.emplace("audio/bgm", ".ogg", m_audio.name());
      m_audioType = AudioType::BGM;
    } else if (code == EventCode::Change_Defeat_ME || code == EventCode::Change_Victory_ME || code == EventCode::Play_ME) {
      m_audioDir.emplace("audio/me", ".ogg", m_audio.name());
      m_audioType = AudioType::ME;
    } else if (code == EventCode::Play_BGS) {
      m_audioDir.emplace("audio/bgs", ".ogg", m_audio.name());
      m_audioType = AudioType::BGS;
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
  std::tuple<bool, bool> draw(std::string_view title, std::string_view className);

  void setOpen(const bool open) {
    m_open = open;
    m_confirmed = false;
  }

private:
  bool m_confirmed{false};
  bool m_open{false};
  AudioType m_audioType;

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
    switch (m_audioType) {
    case AudioType::SE:
      m_sound = Sound(ResourceManager::instance()->loadSE(path));
      break;
    case AudioType::BGM:
      m_sound = Sound(ResourceManager::instance()->loadBGM(path));
      break;
    case AudioType::ME:
      m_sound = Sound(ResourceManager::instance()->loadME(path));
      break;
    case AudioType::BGS:
      m_sound = Sound(ResourceManager::instance()->loadBGS(path));
      break;
    case AudioType::Vocals:
      // TODO
      break;
    }

    setVolume(m_audio.volume());
    setPanning(m_audio.pan());
    setPitch(m_audio.pitch());
    m_sound.play();
    return true;
  }
  void setVolume(const int volume) {
    m_sound.setVolume(static_cast<float>(volume)); // 0% to 100%
  }
  void setPanning(const int value) { m_sound.setPan(static_cast<float>(value) / 100.0f); }
  void setPitch(const int value) { m_sound.setPitch(value / 100.f); }
  void stopAudio() { m_sound.stop(); }
};
