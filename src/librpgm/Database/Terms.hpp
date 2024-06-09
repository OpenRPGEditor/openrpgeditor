#pragma once

#include "Database/Messages.hpp"

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

struct Terms {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Terms, basic, commands, messages, params);
  std::vector<std::optional<std::string>> basic;
  std::vector<std::optional<std::string>> commands;
  Messages messages;
  std::vector<std::optional<std::string>> params;
};