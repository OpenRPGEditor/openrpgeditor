#pragma once

#include <map>
#include <string>
#include <nlohmann/json.hpp>

struct Templates {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Templates, commands);
  std::vector<std::string> commands;

  static Templates load(std::string_view path);
  bool serialize(std::string_view path);
};
