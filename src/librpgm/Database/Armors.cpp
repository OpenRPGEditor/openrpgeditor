#include "Armors.hpp"

#include <fstream>

using json = nlohmann::json;

Armors Armors::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Armors armors;
  armors.m_armors.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    Armor& armor = armors.m_armors.emplace_back();
    value.get_to(armor);
  }
  return armors;
}

bool Armors::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data{nullptr};

  for (const Armor& armor : m_armors) {
    data.push_back(armor);
  }

  file << data;

  return true;
}
