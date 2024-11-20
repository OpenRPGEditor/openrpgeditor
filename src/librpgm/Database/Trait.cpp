#include "Database/Trait.hpp"

void to_json(nlohmann::json& j, const Trait& t) {
  j = {
      {"code", t.code},
      {"dataId", t.dataId},
      {"value", t.value},
  };
}

void from_json(const nlohmann::json& j, Trait& t) {
  t.code = j.value("code", t.code);
  t.dataId = j.value("dataId", t.dataId);
  t.value = j.value("value", t.value);
}