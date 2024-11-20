#include "Database/Vehicle.hpp"

void to_json(nlohmann::json& j, const Vehicle& v) {
  j = {
      {"bgm", v.bgm},
      {"characterIndex", v.characterIndex},
      {"characterName", v.characterName},
      {"startMapId", v.startMapId},
      {"startX", v.startX},
      {"startY", v.startY},
  };
}
void from_json(const nlohmann::json& j, Vehicle& v) {
  v.bgm = j.value("bgm", v.bgm);
  v.characterIndex = j.value("characterIndex", v.characterIndex);
  v.characterName = j.value("characterName", v.characterName);
  v.startMapId = j.value("startMapId", v.startMapId);
  v.startX = j.value("startX", v.startX);
  v.startY = j.value("startY", v.startY);
}
