#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeWindowColorCommand final : IEventCommand {
  ChangeWindowColorCommand() = default;
  explicit ChangeWindowColorCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    auto colors = parameters[0];
    colors[0].get_to(r);
    colors[1].get_to(g);
    colors[2].get_to(b);
  }
  ~ChangeWindowColorCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Window_Color; }
  int r;
  int g;
  int b;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
