#pragma once
#include "Database/Globals.hpp"

#include <nlohmann/json.hpp>
#include <string>

struct Effect {
  friend void to_json(nlohmann::ordered_json& to, const Effect& effect);
  friend void from_json(const nlohmann::ordered_json& from, Effect& effect);
  EffectCode code = EffectCode::Recover_HP;
  int dataId{0};     // Object ID depending on the selection (Add State, Remove State, Common Event, Learn Skill)
  float value1{1.f}; // Combobox member value OR dataId's value -- percent values are used as (1.00 ~ -1.00)
  int value2{0};     // Never used for object id; secondary value after combobox
};
void to_json(nlohmann::ordered_json& to, const Effect& effect);
void from_json(const nlohmann::ordered_json& from, Effect& effect);