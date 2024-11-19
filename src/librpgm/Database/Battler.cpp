#include "Database/Battler.hpp"

void to_json(nlohmann::json& out, const Battler& battler) {
  out = {
      {"actorId", battler.actorId},
      {"equips", battler.equips},
      {"level", battler.level},
  };
}
void from_json(const nlohmann::json& from, Battler& battler) {
  battler.actorId = from.value("actorId", battler.actorId);
  battler.equips = from.value("equips", battler.equips);
  battler.level = from.value("level", battler.level);
}