#include "Database/Damage.hpp"

void to_json(nlohmann::ordered_json& j, const Damage& damage) {
  j = {
      {"critical", damage.critical}, {"elementId", damage.elementId}, {"formula", damage.formula},
      {"type", damage.type},         {"variance", damage.variance},
  };
}
void from_json(const nlohmann::ordered_json& j, Damage& damage) {
  damage.critical = j.value("critical", damage.critical);
  damage.elementId = j.value("elementId", damage.elementId);
  damage.formula = j.value("formula", damage.formula);
  damage.type = j.value("type", damage.type);
  damage.variance = j.value("variance", damage.variance);
}