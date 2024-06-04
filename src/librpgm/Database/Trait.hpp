#pragma once

#include <nlohmann/json.hpp>

struct Trait {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Trait, code, dataId, value);
  int code;
  int dataId;
  double value;
};