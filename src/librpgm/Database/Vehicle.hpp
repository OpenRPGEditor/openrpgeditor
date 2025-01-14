#pragma once

#include "Database/Audio.hpp"

#include <nlohmann/json.hpp>

class Vehicle final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& j, const Vehicle& v);
  friend void from_json(const nlohmann::ordered_json& j, Vehicle& v);

public:
  Vehicle() = default;
  Vehicle(const Vehicle& other);
  Vehicle& operator=(const Vehicle& other);
  Vehicle(Vehicle&& other) noexcept;
  Vehicle& operator=(Vehicle&& other) noexcept;

  Audio& bgm();
  const Audio& bgm() const;
  void setBgm(const Audio& bgm);

  int characterIndex() const;
  void setCharacterIndex(int characterIndex);

  const std::string& characterName() const;
  void setCharacterName(const std::string& characterName);

  int startMapId() const;
  void setStartMapId(int startMapId);

  int startX() const;
  void setStartX(int startX);

  int startY() const;
  void setStartY(int startY);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Vehicle*, const Audio&)>& bgmModified();
  rpgmutils::signal<void(Vehicle*, int)>& characterIndexModified();
  rpgmutils::signal<void(Vehicle*, const std::string&)>& characterNameModified();
  rpgmutils::signal<void(Vehicle*, int)>& startMapIdModified();
  rpgmutils::signal<void(Vehicle*, int)>& startXModified();
  rpgmutils::signal<void(Vehicle*, int)>& startYModified();

  bool operator==(const Vehicle& v) const {
    return m_bgm == v.m_bgm && m_characterIndex == v.m_characterIndex && m_characterName == v.m_characterName && m_startMapId == v.m_startMapId && m_startX == v.m_startX && m_startY == v.m_startY;
  }

private:
  Audio m_bgm;
  int m_characterIndex;
  std::string m_characterName;
  int m_startMapId;
  int m_startX;
  int m_startY;

  std::optional<Audio> m_oldbgm;
  std::optional<int> m_oldcharacterIndex;
  std::optional<std::string> m_oldcharacterName;
  std::optional<int> m_oldstartMapId;
  std::optional<int> m_oldstartX;
  std::optional<int> m_oldstartY;

  std::optional<rpgmutils::signal<void(Vehicle*, const Audio&)>> m_bgmModified;
  std::optional<rpgmutils::signal<void(Vehicle*, int)>> m_characterIndexModified;
  std::optional<rpgmutils::signal<void(Vehicle*, const std::string&)>> m_characterNameModified;
  std::optional<rpgmutils::signal<void(Vehicle*, int)>> m_startMapIdModified;
  std::optional<rpgmutils::signal<void(Vehicle*, int)>> m_startXModified;
  std::optional<rpgmutils::signal<void(Vehicle*, int)>> m_startYModified;
};
void to_json(nlohmann::ordered_json& j, const Vehicle& v);
void from_json(const nlohmann::ordered_json& j, Vehicle& v);
