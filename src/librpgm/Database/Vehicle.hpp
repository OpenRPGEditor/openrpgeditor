#pragma once

#include "Database/Audio.hpp"

#include <nlohmann/json.hpp>

class Vehicle {
public:
  friend void to_json(nlohmann::json& j, const Vehicle& v);
  friend void from_json(const nlohmann::json& j, Vehicle& v);
  Audio bgm;
  int characterIndex;
  std::string characterName;
  int startMapId;
  int startX;
  int startY;
};
void to_json(nlohmann::json& j, const Vehicle& v);
void from_json(const nlohmann::json& j, Vehicle& v);
