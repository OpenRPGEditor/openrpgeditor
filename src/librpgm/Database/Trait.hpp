#pragma once

#include <nlohmann/json.hpp>

struct Trait {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Trait, code, dataId, value);
  int code;
  int dataId;
  double value;
};