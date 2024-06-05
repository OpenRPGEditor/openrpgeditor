#include "Database/Map.hpp"
#include <fstream>

Map Map::load(std::string_view filepath) {
  std::ifstream file(filepath.data());
  return nlohmann::json::parse(file).get<Map>();
}

bool Map::serialize(std::string_view filepath) {
#if 0
  std::ofstream file(filepath.data());
  nlohmann::json data = *this;
  file << data;
#endif
  return false;
}
