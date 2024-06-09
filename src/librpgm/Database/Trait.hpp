#pragma once

#include "Database/Globals.hpp"

#include "nlohmann/json.hpp"

struct Trait {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Trait, code, dataId, value);
  TraitCode code;
  int dataId;
  double value;
};