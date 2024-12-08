#include "Database/Tileset.hpp"

void to_json(nlohmann::ordered_json& j, const Tileset& tileset) {
  j = {
      {"id", tileset.m_id}, {"flags", tileset.m_flags}, {"mode", tileset.m_mode}, {"name", tileset.m_name}, {"note", tileset.m_note}, {"tilesetNames", tileset.m_tilesetNames},
  };
}

void from_json(const nlohmann::ordered_json& j, Tileset& tileset) {
  tileset.m_id = j.value("id", tileset.m_id);
  tileset.m_flags = j.value("flags", tileset.m_flags);
  tileset.m_mode = j.value("mode", tileset.m_mode);
  tileset.m_name = j.value("name", tileset.m_name);
  tileset.m_note = j.value("note", tileset.m_note);
  tileset.m_tilesetNames = j.value("tilesetNames", tileset.m_tilesetNames);
}
