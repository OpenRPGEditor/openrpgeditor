#include "Database/Templates.hpp"

#include "Database/Database.hpp"

#include <format>
#include <fstream>

void to_json(nlohmann::ordered_json& j, const Templates& templ) {
  j = {
      {"templates", templ.templates},
  };
}

void from_json(const nlohmann::ordered_json& j, Templates& templ) { templ.templates = j.value("templates", templ.templates); }

Templates Templates::load(const std::filesystem::path& path) {
  if (std::ifstream file(path); file.is_open()) {
    try {
      nlohmann::ordered_json data = nlohmann::ordered_json::parse(file);
      Templates ret;
      data.get_to(ret);
      return ret;
    } catch (...) {}
  }

  return {};
}

bool Templates::serialize(const std::filesystem::path& path) {
  std::ofstream file(path);
  try {
    if (file.is_open()) {
      const nlohmann::ordered_json data = *this;
      file << data.dump(4);
      return true;
    }
  } catch (...) {}
  return false;
}