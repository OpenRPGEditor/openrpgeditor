#include "Actors.hpp"

#include <fstream>

using json = nlohmann::json;

Actors Actors::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
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
  json data;

  for (const Actor& actor : m_actors) {
    if (actor.m_isValid) {
      data.push_back(actor);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data;
  return true;
}
