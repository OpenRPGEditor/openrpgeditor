#include "Database/Audio.hpp"

Audio::Audio(const Audio& other)
: IModifiable(other)
, m_name(other.m_name)
, m_pan(other.m_pan)
, m_pitch(other.m_pitch)
, m_volume(other.m_volume)
, m_oldname(other.m_oldname)
, m_oldpan(other.m_oldpan)
, m_oldvolume(other.m_oldvolume) {}

Audio& Audio::operator=(const Audio& other) {
  m_name = other.m_name;
  m_pan = other.m_pan;
  m_pitch = other.m_pitch;
  m_volume = other.m_volume;
  m_oldname = other.m_oldname;
  m_oldpan = other.m_oldpan;
  m_oldvolume = other.m_oldvolume;
  return *this;
}

Audio::Audio(Audio&& other) noexcept
: IModifiable(std::move(other))
, m_name(std::move(other.m_name))
, m_pan(other.m_pan)
, m_pitch(other.m_pitch)
, m_volume(other.m_volume)
, m_oldname(std::move(other.m_oldname))
, m_oldpan(other.m_oldpan)
, m_oldvolume(other.m_oldvolume) {}

Audio& Audio::operator=(Audio&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_name = std::move(other.m_name);
  m_pan = other.m_pan;
  m_pitch = other.m_pitch;
  m_volume = other.m_volume;
  m_oldname = std::move(other.m_oldname);
  m_oldpan = other.m_oldpan;
  m_oldvolume = other.m_oldvolume;
  return *this;
}

const std::string& Audio::name() const { return m_name; }
void Audio::setName(const std::string& name) {
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    emit_signal(nameModified(), this, name);
  }
  setModified();
}

int Audio::pan() const { return m_pan; }
void Audio::setPan(int pan) {
  MODIFIABLE_SET_OLD_VALUE(pan);
  m_pan = pan;
  if (!signalsDisabled()) {
    emit_signal(panModified(), this, pan);
  }
  setModified();
}

int Audio::pitch() const { return m_pitch; }
void Audio::setPitch(int pitch) {
  MODIFIABLE_SET_OLD_VALUE(pitch);
  m_pitch = pitch;
  if (!signalsDisabled()) {
    emit_signal(pitchModified(), this, pitch);
  }
  setModified();
}
int Audio::volume() const { return m_volume; }
void Audio::setVolume(int volume) {
  MODIFIABLE_SET_OLD_VALUE(volume);
  m_volume = volume;
  if (!signalsDisabled()) {
   emit_signal( volumeModified(), this, volume);
  }
  setModified();
}

void Audio::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(pan);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(pitch);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(volume);
}

void Audio::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(pan);
  MODIFIABLE_ACCEPT_VALUE(pitch);
  MODIFIABLE_ACCEPT_VALUE(volume);
}

nlohmann::ordered_json Audio::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(name),
      MODIFIABLE_SERIALIZE_VALUE(pan),
      MODIFIABLE_SERIALIZE_VALUE(pitch),
      MODIFIABLE_SERIALIZE_VALUE(volume),
  };
}

rpgmutils::signal<void(Audio*, std::string_view)>& Audio::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}

rpgmutils::signal<void(Audio*, int)>& Audio::panModified() {
  if (!m_panModified) {
    m_panModified.emplace();
  }
  return *m_panModified;
}

rpgmutils::signal<void(Audio*, int)>& Audio::pitchModified() {
  if (!m_pitchModified) {
    m_pitchModified.emplace();
  }
  return *m_pitchModified;
}

rpgmutils::signal<void(Audio*, int)>& Audio::volumeModified() {
  if (!m_volumeModified) {
    m_volumeModified.emplace();
  }
  return *m_volumeModified;
}

void to_json(nlohmann::ordered_json& out, const Audio& audio) {
  out = {
      {"name", audio.m_name},
      {"pan", audio.m_pan},
      {"pitch", audio.m_pitch},
      {"volume", audio.m_volume},
  };
}

void from_json(const nlohmann::ordered_json& from, Audio& audio) {
  audio.m_name = from.value("name", audio.m_name);
  audio.m_pan = from.value("pan", audio.m_pan);
  audio.m_pitch = from.value("pitch", audio.m_pitch);
  audio.m_volume = from.value("volume", audio.m_volume);
}