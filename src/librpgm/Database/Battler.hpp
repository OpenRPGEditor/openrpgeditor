#pragma once
#include "nlohmann/json.hpp"

class Battler {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Battler, actorId, equips, level);
  int actorId{};
  std::vector<int> equips;
  int level{};
};
