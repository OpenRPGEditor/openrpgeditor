#pragma once

#include <nlohmann/json.hpp>
#include <string>

struct Effect {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Effect, code, dataId, value1, value2);
  // TODO: Fully document this
  int code;
  int dataId;
  int value1;
  int value2;
};