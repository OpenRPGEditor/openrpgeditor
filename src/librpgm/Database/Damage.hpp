#pragma once
#include "Database/Globals.hpp"

#include <nlohmann/json.hpp>
#include <string>

struct Damage {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Damage, critical, elementId, formula, type, variance);

  bool critical;
  int elementId;
  std::string formula;
  DamageType type;
  int variance;
};
