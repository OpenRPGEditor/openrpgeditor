#include "Database/Templates.hpp"

#include "Database/Database.hpp"

#include <fstream>
#include <format>

Templates Templates::load(std::string_view path) {
  if (std::ifstream file(path.data()); file.is_open()) {
    try {
      nlohmann::json data = nlohmann::json::parse(file);
      Templates ret;
      data.get_to(ret);
      return ret;
    } catch (...) {}
  }

  return {};
}

bool Templates::serialize(const std::string_view path) {
  std::ofstream file(path.data());
  try {
    if (file.is_open()) {
      const nlohmann::json data = *this;
      file << data.dump(4);
      return true;
    }
  } catch (...) {}
  return false;
}