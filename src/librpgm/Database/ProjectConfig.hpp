#pragma once

#include "Database/Globals.hpp"
#include "nlohmann/json.hpp"

#include <deque>

struct ProjectConfig {

  ProjectConfig();
  bool load(const std::filesystem::path& path);
  bool loadFromJson(const nlohmann::ordered_json& parser);
  void serialize(const std::filesystem::path& path);
  nlohmann::ordered_json serializeToJson();
  std::map<int, MapStateType> mapStateList;

  [[nodiscard]] static ProjectConfig* instance() { return m_instance; }

private:
  static ProjectConfig* m_instance;
};

struct TransientConfig {
  TransientConfig();
  bool load(const std::filesystem::path& path);
  bool loadFromJson(const nlohmann::ordered_json& parser);
  void serialize(const std::filesystem::path& path);
  nlohmann::ordered_json serializeToJson();
  std::string imguiState;

  [[nodiscard]] static TransientConfig* instance() { return m_instance; }

private:
  static TransientConfig* m_instance;
};