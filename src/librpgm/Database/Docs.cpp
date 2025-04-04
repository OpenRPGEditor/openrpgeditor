#include "Database/Docs.hpp"

#include "Database/Database.hpp"

#include <format>
#include <fstream>

void to_json(nlohmann::ordered_json& j, const Docs& doc) {
  j = {
      {"docs", doc.docs},
  };
}

void from_json(const nlohmann::ordered_json& j, Docs& doc) { doc.docs = j.value("docs", doc.docs); }

Docs Docs::load(std::string_view path) {
  if (std::ifstream file(path.data()); file.is_open()) {
    try {
      nlohmann::ordered_json data = nlohmann::ordered_json::parse(file);
      Docs ret;
      data.get_to(ret);
      return ret;
    } catch (...) {}
  }

  return {};
}

bool Docs::serialize(const std::string_view path) {
  std::ofstream file(path.data());
  try {
    if (file.is_open()) {
      const nlohmann::ordered_json data = *this;
      file << data.dump(4);
      return true;
    }
  } catch (...) {}
  return false;
}