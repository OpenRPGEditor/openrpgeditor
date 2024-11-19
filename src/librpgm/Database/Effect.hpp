#pragma once

#include <nlohmann/json.hpp>
#include <string>

struct Effect {
  friend void to_json(nlohmann::json& to, const Effect& effect);
  friend void from_json(const nlohmann::json& from, Effect& effect);
  // TODO: Fully document this
  int code{};
  int dataId{};
  int value1{};
  int value2{};
};
void to_json(nlohmann::json& to, const Effect& effect);
void from_json(const nlohmann::json& from, Effect& effect);