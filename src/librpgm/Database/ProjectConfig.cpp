#include "Database/ProjectConfig.hpp"

#include <fstream>

ProjectConfig* ProjectConfig::m_instance = nullptr;

ProjectConfig::ProjectConfig() { m_instance = this; }
bool ProjectConfig::load(const std::filesystem::path& path) {
  try {
    if (std::ifstream f(path); f.is_open()) {
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
void ProjectConfig::serialize(const std::filesystem::path& path) {
  if (!exists(path.parent_path())) {
    create_directories(path.parent_path());
  }

  if (std::ofstream f(path); f.is_open()) {
    f << serializeToJson().dump(4);
  }
}

nlohmann::ordered_json ProjectConfig::serializeToJson() {
  return {
      {"mapStateList", mapStateList}, //
  };
}

TransientConfig* TransientConfig::m_instance = nullptr;

TransientConfig::TransientConfig() { m_instance = this; }

bool TransientConfig::load(const std::filesystem::path& path) {
  try {
    if (std::ifstream f(path); f.is_open()) {
      return loadFromJson(nlohmann::ordered_json::parse(f));
    }
  } catch (...) {
    // scream into the void, we don't care
  }
  return false;
}

bool TransientConfig::loadFromJson(const nlohmann::ordered_json& parser) {
  try {
    imguiState = parser.value("imguiState", imguiState);
    return true;
  } catch (...) {}

  return false;
}
void TransientConfig::serialize(const std::filesystem::path& path) {
  if (!exists(path.parent_path())) {
    create_directories(path.parent_path());
  }

  if (std::ofstream f(path); f.is_open()) {
    f << serializeToJson().dump(4);
  }
}

nlohmann::ordered_json TransientConfig::serializeToJson() {
  return {
      {"imguiState", imguiState},
  };
}