#pragma once

#include "Database/Messages.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class Terms final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& j, const Terms& terms);
  friend void from_json(const nlohmann::ordered_json& j, Terms& terms);

public:
  std::vector<std::optional<std::string>> basic;
  std::vector<std::optional<std::string>> commands;
  Messages messages;
  std::vector<std::optional<std::string>> params;

  bool operator==(const Terms& rhs) const { return basic == rhs.basic && commands == rhs.commands && params == rhs.params && messages == rhs.messages; }
};

void to_json(nlohmann::ordered_json& j, const Terms& terms);
void from_json(const nlohmann::ordered_json& j, Terms& terms);