#pragma once
#include "Database/RPGEnums.hpp"

#include <nlohmann/json.hpp>
#include <string>

struct Damage {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Damage, critical, elementId, formula, type, variance);

  bool critical;
  int elementId;
  std::string formula;
  DamageType type;
  int variance;
};
