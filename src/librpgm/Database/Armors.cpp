#include "Armors.hpp"

#include <fstream>

using json = nlohmann::ordered_json;

void to_json(nlohmann::ordered_json& out, const Armor& armor) {
  out = {
      {"id", armor.id},     {"atypeId", armor.atypeId}, {"description", armor.description}, {"etypeId", armor.etypeId}, {"traits", armor.traits}, {"iconIndex", armor.iconIndex},
      {"name", armor.name}, {"note", armor.note},       {"params", armor.params},           {"price", armor.price},
  };
}

void from_json(const nlohmann::ordered_json& from, Armor& armor) {
  armor.id = from.value("id", armor.id);
  armor.atypeId = from.value("atypeId", armor.atypeId);
  armor.description = from.value("description", armor.description);
  armor.etypeId = from.value("etypeId", armor.etypeId);
  armor.traits = from.value("traits", armor.traits);
  armor.iconIndex = from.value("iconIndex", armor.iconIndex);
  armor.name = from.value("name", armor.name);
  armor.note = from.value("note", armor.note);
  armor.params = from.value("params", armor.params);
  armor.price = from.value("price", armor.price);
}

Armors Armors::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Armors armors;
  armors.m_armors.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Armor& armor = armors.m_armors.emplace_back();
    armor.m_isValid = value != nullptr;
    if (armor.m_isValid) {
      value.get_to(armor);
    } else {
      armor.id = i;
    }
    ++i;
  }
  return armors;
}

bool Armors::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data;

  for (const Armor& armor : m_armors) {
    if (armor.m_isValid) {
      data.push_back(armor);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data.dump(4);

  return true;
}
