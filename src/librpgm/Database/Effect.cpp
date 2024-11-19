#include "Database/Effect.hpp"

void to_json(nlohmann::json& to, const Effect& effect) {
  to = {
      {"code", effect.code},
      {"dataId", effect.dataId},
      {"value1", effect.value1},
      {"value2", effect.value2},
  };
}

void from_json(const nlohmann::json& from, Effect& effect) {
  effect.code = from.value("code", effect.code);
  effect.dataId = from.value("dataId", effect.dataId);
  effect.value1 = from.value("value1", effect.value1);
  effect.value2 = from.value("value2", effect.value2);
}