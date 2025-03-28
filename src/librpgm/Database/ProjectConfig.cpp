#include "Database/ProjectConfig.hpp"

#include <fstream>

ProjectConfig* ProjectConfig::m_instance = nullptr;

ProjectConfig::ProjectConfig() { m_instance = this; }
bool ProjectConfig::load(std::string_view path) {
  try {
    if (std::ifstream f(path.data()); f.is_open()) {
      return loadFromJson(nlohmann::ordered_json::parse(f));
    }
  } catch (...) {
    // scream into the void, we don't care
  }
  return false;
}

bool ProjectConfig::loadFromJson(const nlohmann::ordered_json& parser) {
  try {
    mapStateList = parser.value("mapStateList", mapStateList);
    return true;
  } catch (...) {}

  return false;
}
void ProjectConfig::serialize(const std::string_view path) {
  if (std::ofstream f(path.data()); f.is_open()) {
    f << serializeToJson().dump(4);
  }
}

nlohmann::ordered_json ProjectConfig::serializeToJson() {
  return {
        {"mapStateList", mapStateList}, //

    };
}