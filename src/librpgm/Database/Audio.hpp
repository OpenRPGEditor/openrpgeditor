#pragma once

#include "Database/IModifiable.hpp"

#include <string>

#include "nlohmann/json.hpp"

class Audio final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& out, const Audio& audio);
  friend void from_json(const nlohmann::ordered_json& from, Audio& audio);

public:
  Audio() = default;
  Audio(const Audio& other);
  Audio& operator=(const Audio& other);
  Audio(Audio&& other) noexcept;
  Audio& operator=(Audio&& other) noexcept;

  const std::string& name() const;
  void setName(const std::string& name);

  int pan() const;
  void setPan(int pan);

  int pitch() const;
  void setPitch(int pitch);

  int volume() const;
  void setVolume(int volume);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Audio*, std::string_view)>& nameModified();
  rpgmutils::signal<void(Audio*, int)>& panModified();
  rpgmutils::signal<void(Audio*, int)>& pitchModified();
  rpgmutils::signal<void(Audio*, int)>& volumeModified();

private:
  std::string m_name; // base name of the file (without extension) in audio/se
  int m_pan = 0;
  int m_pitch = 100;
  int m_volume = 100;

  std::optional<std::string> m_oldname;
  std::optional<int> m_oldpan;
  std::optional<int> m_oldpitch;
  std::optional<int> m_oldvolume;

  std::optional<rpgmutils::signal<void(Audio*, std::string_view)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Audio*, int)>> m_panModified;
  std::optional<rpgmutils::signal<void(Audio*, int)>> m_pitchModified;
  std::optional<rpgmutils::signal<void(Audio*, int)>> m_volumeModified;
};
void to_json(nlohmann::json& out, const Audio& audio);
void from_json(const nlohmann::json& from, Audio& audio);