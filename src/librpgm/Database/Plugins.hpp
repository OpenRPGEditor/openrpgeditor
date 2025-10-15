#pragma once

#include <string>

#include "nlohmann/json.hpp"

struct Plugin {
  friend void to_json(nlohmann::ordered_json& to, const Plugin& plugin);
  friend void from_json(const nlohmann::ordered_json& from, Plugin& plugin);

  std::string name;
  bool status{true};
  std::string description;
  std::vector<std::pair<std::string, std::string>> parameters;
};

struct Plugins {
  static Plugins load(const std::filesystem::path& path);
  bool serialize(const std::filesystem::path& path) const;

  std::vector<Plugin> plugins;
};
