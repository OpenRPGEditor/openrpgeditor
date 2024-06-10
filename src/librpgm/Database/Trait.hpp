#pragma once

#include "Database/Globals.hpp"

#include "nlohmann/json.hpp"

struct Trait {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Trait, code, dataId, value);
  TraitCode code = TraitCode::Element_Rate;
  int dataId = 1;
  double value = 1.f;
};