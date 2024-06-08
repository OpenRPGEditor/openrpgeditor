#include "Actors.hpp"

#include <fstream>

#include <imgui.h>

using json = nlohmann::json;

Actors::Actors() : m_isOpen(true) {}

Actors Actors::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Actors actors;
  actors.m_actors.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    Actor& actor = actors.m_actors.emplace_back();
    value.get_to(actor);
  }
  return actors;
}

bool Actors::serialize(std::string_view filename) {
  std::ofstream file(filename.data());
  json data;

  for (const Actor& actor : m_actors) {
    data.push_back(actor);
  }

  file << data;
  return true;
}
