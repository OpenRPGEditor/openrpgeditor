#pragma once

#include "Database/Globals.hpp"
#include "nlohmann/json.hpp"

#include <deque>

struct ProjectConfig {

  ProjectConfig();
  bool load(std::string_view path);
  bool loadFromJson(const nlohmann::ordered_json& parser);
  void serialize(std::string_view path);
  nlohmann::ordered_json serializeToJson();
  std::map<int, MapStateType> mapStateList;

  [[nodiscard]] static ProjectConfig* instance() { return m_instance; }

private:
  static ProjectConfig* m_instance;
};