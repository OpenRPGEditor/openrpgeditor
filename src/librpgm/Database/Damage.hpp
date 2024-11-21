#pragma once
#include "Database/Globals.hpp"

#include <nlohmann/json.hpp>
#include <string>

struct Damage {
  friend void to_json(nlohmann::ordered_json& j, const Damage& damage);
  friend void from_json(const nlohmann::ordered_json& j, Damage& damage);

  bool critical;
  int elementId;
  std::string formula;
  DamageType type;
  int variance;
};

void to_json(nlohmann::ordered_json& j, const Damage& damage);
void from_json(const nlohmann::ordered_json& j, Damage& damage);