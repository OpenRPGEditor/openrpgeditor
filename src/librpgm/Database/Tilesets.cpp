#include "Tilesets.hpp"

#include <fstream>

using json = nlohmann::json;

Tileset::Tileset() {}

Tilesets Tilesets::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Tilesets tilesets;
  tilesets.m_tilesets.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    Tileset& tileset = tilesets.m_tilesets.emplace_back();
    value.get_to(tileset);
  }
  return tilesets;
}

bool Tilesets::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  for (const Tileset& tileset : m_tilesets) {
    data.push_back(tileset);
  }

  file << data;

  return true;
}
