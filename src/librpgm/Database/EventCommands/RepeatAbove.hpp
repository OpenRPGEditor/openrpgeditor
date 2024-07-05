#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct RepeatAboveCommand final : IEventCommand {
  RepeatAboveCommand() = default;
  explicit RepeatAboveCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~RepeatAboveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Repeat_Above; }
};
