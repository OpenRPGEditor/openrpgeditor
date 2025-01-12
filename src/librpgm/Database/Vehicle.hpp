#pragma once

#include "Database/Audio.hpp"

#include <nlohmann/json.hpp>

class Vehicle final : public IModifiable {
public:
  friend void to_json(nlohmann::ordered_json& j, const Vehicle& v);
  friend void from_json(const nlohmann::ordered_json& j, Vehicle& v);
  Audio bgm;
  int characterIndex;
  std::string characterName;
  int startMapId;
  int startX;
  int startY;

  bool operator==(const Vehicle& v) const {
    return bgm == v.bgm && characterIndex == v.characterIndex && characterName == v.characterName && startMapId == v.startMapId && startX == v.startX && startY == v.startY;
  }
};
void to_json(nlohmann::ordered_json& j, const Vehicle& v);
void from_json(const nlohmann::ordered_json& j, Vehicle& v);
