#include "Tilesets.hpp"

#include <fstream>

using json = nlohmann::ordered_json;

void to_json(nlohmann::ordered_json& j, const Tileset& tileset) {
  j = {
      {"id", tileset.id}, {"flags", tileset.flags}, {"mode", tileset.mode}, {"name", tileset.name}, {"note", tileset.note}, {"tilesetNames", tileset.tilesetNames},
  };
}

void from_json(const nlohmann::ordered_json& j, Tileset& tileset) {
  tileset.id = j.value("id", tileset.id);
  tileset.flags = j.value("flags", tileset.flags);
  tileset.mode = j.value("mode", tileset.mode);
  tileset.name = j.value("name", tileset.name);
  tileset.note = j.value("note", tileset.note);
  tileset.tilesetNames = j.value("tilesetNames", tileset.tilesetNames);
}

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
