#pragma once
#include "nlohmann/json.hpp"

class Battler {
  friend void to_json(nlohmann::ordered_json& out, const Battler& battler);
  friend void from_json(const nlohmann::ordered_json& from, Battler& battler);
  int actorId{};
  std::vector<int> equips;
  int level{};
};

void to_json(nlohmann::ordered_json& out, const Battler& battler);
void from_json(const nlohmann::ordered_json& from, Battler& battler);