#include "Armors.hpp"

#include <fstream>

using json = nlohmann::json;

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

  file << data;

  return true;
}
