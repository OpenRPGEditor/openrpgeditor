#pragma once

#include "Database/Globals.hpp"

#include "nlohmann/json.hpp"

struct Trait {
  friend void to_json(nlohmann::ordered_json& j, const Trait& trait);
  friend void from_json(const nlohmann::ordered_json& j, Trait& trait);
  TraitCode code = TraitCode::Element_Rate;
  int dataId = 1;
  double value = 1.f;
};
void to_json(nlohmann::ordered_json& j, const Trait& trait);
void from_json(const nlohmann::ordered_json& j, Trait& trait);