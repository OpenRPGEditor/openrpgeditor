#pragma once

#include <string>

#include "nlohmann/json.hpp"

struct Plugin {
  friend void to_json(nlohmann::json& to, const Plugin& plugin);
  friend void from_json(const nlohmann::json& from, Plugin& plugin);

  std::string name;
  bool status{true};
  std::string description;
  std::vector<std::pair<std::string, std::string>> parameters;
};

struct Plugins {
  static Plugins load(std::string_view path);
  bool serialize(std::string_view path);

  std::vector<Plugin> plugins;
};
