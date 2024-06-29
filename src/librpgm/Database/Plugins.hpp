#pragma once

#include <string>

#include "nlohmann/json.hpp"

struct Plugin {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Plugin, name, status, description, parameters);
  std::string name;
  bool status{true};
  std::string description;
  std::map<std::string, std::string> parameters;
};

struct Plugins {
  static Plugins load(std::string_view path);

  std::vector<Plugin> plugins;
};
