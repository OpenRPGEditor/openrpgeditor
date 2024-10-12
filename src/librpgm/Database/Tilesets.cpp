#include "Tilesets.hpp"

#include <fstream>

using json = nlohmann::json;

Tileset::Tileset() {}

Tilesets Tilesets::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Tilesets tilesets;
  tilesets.m_tilesets.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Tileset& tileset = tilesets.m_tilesets.emplace_back();
    tileset.m_isValid = value != nullptr;
    if (tileset.m_isValid) {
      value.get_to(tileset);
    } else {
      tileset.id = i;
    }
    ++i;
  }
  return tilesets;
}

bool Tilesets::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data;

  for (const Tileset& tileset : m_tilesets) {
    if (tileset.m_isValid) {
      data.push_back(tileset);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data.dump(4);

  return true;
}
