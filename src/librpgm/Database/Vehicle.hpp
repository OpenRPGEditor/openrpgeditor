#pragma once

#include "Database/Audio.hpp"

#include <nlohmann/json.hpp>

class Vehicle {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Vehicle, bgm, characterIndex, characterName, startMapId, startX, startY);
  Audio bgm;
  int characterIndex;
  std::string characterName;
  int startMapId;
  int startX;
  int startY;
};
