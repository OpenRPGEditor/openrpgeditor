#include "Actors.hpp"

#include <fstream>

void to_json(nlohmann::ordered_json& to, const Actor& actor) {
  to = {
      {"id", actor.id},
      {"battlerName", actor.battlerName},
      {"characterIndex", actor.characterIndex},
      {"characterName", actor.characterName},
      {"classId", actor.classId},
      {"equips", actor.equips},
      {"faceIndex", actor.faceIndex},
      {"faceName", actor.faceName},
      {"traits", actor.traits},
      {"initialLevel", actor.initialLevel},
      {"maxLevel", actor.maxLevel},
      {"name", actor.name},
      {"nickname", actor.nickname},
      {"note", actor.note},
      {"profile", actor.profile},
  };
}
void from_json(const nlohmann::ordered_json& from, Actor& actor) {
  actor.id = from.value("id", actor.id);
  actor.battlerName = from.value("battlerName", actor.battlerName);
  actor.characterIndex = from.value("characterIndex", actor.characterIndex);
  actor.characterName = from.value("characterName", actor.characterName);
  actor.classId = from.value("classId", actor.classId);
  actor.equips = from.value("equips", actor.equips);
  actor.faceIndex = from.value("faceIndex", actor.faceIndex);
  actor.faceName = from.value("faceName", actor.faceName);
  actor.traits = from.value("traits", actor.traits);
  actor.initialLevel = from.value("initialLevel", actor.initialLevel);
  actor.maxLevel = from.value("maxLevel", actor.maxLevel);
  actor.name = from.value("name", actor.name);
  actor.nickname = from.value("nickname", actor.nickname);
  actor.profile = from.value("profile", actor.profile);
}

Actors Actors::load(std::string_view filename) {
  std::ifstream file(filename.data());
  nlohmann::ordered_json data = nlohmann::ordered_json::parse(file);
  Actors actors;
  actors.m_actors.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Actor& actor = actors.m_actors.emplace_back();
    actor.m_isValid = value != nullptr;
    if (actor.m_isValid) {
      value.get_to(actor);
    } else {
      actor.id = i;
    }
    ++i;
  }
  return actors;
}

bool Actors::serialize(std::string_view filename) {
  std::ofstream file(filename.data());
  nlohmann::ordered_json data;

  for (const Actor& actor : m_actors) {
    if (actor.m_isValid) {
      data.push_back(actor);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data.dump(4);
  return true;
}
