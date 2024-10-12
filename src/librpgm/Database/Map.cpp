#include "Database/Map.hpp"
#include <fstream>

Map Map::load(std::string_view filepath) {
  std::ifstream file(filepath.data());
  if (file.is_open()) {
    Map ret = nlohmann::json::parse(file).get<Map>();
    ret.m_isValid = true;
    return ret;
  }
  return Map();
}

bool Map::serialize(std::string_view filepath) const {
  std::ofstream file(filepath.data());

  if (file.is_open()) {
    nlohmann::json data = *this;
    file << data.dump(4);
    return true;
  }

  return false;
}
