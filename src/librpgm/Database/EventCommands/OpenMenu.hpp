#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct OpenMenuCommand final : IEventCommand {
  OpenMenuCommand() = default;
  explicit OpenMenuCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~OpenMenuCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Open_Menu_Screen; }
};
