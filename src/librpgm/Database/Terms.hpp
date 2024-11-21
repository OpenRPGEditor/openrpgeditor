#pragma once

#include "Database/Messages.hpp"

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

struct Terms {
  friend void to_json(nlohmann::ordered_json &j, const Terms &terms);
  friend void from_json(const nlohmann::ordered_json &j, Terms &terms);
  std::vector<std::optional<std::string>> basic;
  std::vector<std::optional<std::string>> commands;
  Messages messages;
  std::vector<std::optional<std::string>> params;
};

void to_json(nlohmann::ordered_json &j, const Terms &terms);
void from_json(const nlohmann::ordered_json &j, Terms &terms);