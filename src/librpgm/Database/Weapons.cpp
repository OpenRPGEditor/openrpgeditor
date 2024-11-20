#include "Weapons.hpp"

#include <fstream>

using json = nlohmann::json;

void to_json(nlohmann::json& j, const Weapon& w) {
  j = {
      {"id", w.id},
      {"animationId", w.animationId},
      {"description", w.description},
      {"etypeId", w.etypeId},
      {"traits", w.traits},
      {"iconIndex", w.iconIndex},
      {"name", w.name},
      {"note", w.note},
      {"params", w.params},
      {"price", w.price},
      {"wtypeId", w.wtypeId},
  };
}

void from_json(const nlohmann::json& j, Weapon& w) {
  w.id = j.value("id", w.id);
  w.animationId = j.value("animationId", w.animationId);
  w.description = j.value("description", w.description);
  w.etypeId = j.value("etypeId", w.etypeId);
  w.iconIndex = j.value("iconIndex", w.iconIndex);
  w.name = j.value("name", w.name);
  w.note = j.value("note", w.note);
  w.params = j.value("params", w.params);
  w.price = j.value("price", w.price);
  w.wtypeId = j.value("wtypeId", w.wtypeId);
}

Weapons Weapons::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Weapons weapons;
  weapons.m_weapons.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Weapon& weapon = weapons.m_weapons.emplace_back();
    weapon.m_isValid = value != nullptr;
    if (weapon.m_isValid) {
      value.get_to(weapon);
    } else {
      weapon.id = i;
    }
    ++i;
  }
  return weapons;
}

bool Weapons::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  for (const Weapon& weapon : m_weapons) {
    data.push_back(weapon);
  }

  file << data.dump(4);

  return true;
}
