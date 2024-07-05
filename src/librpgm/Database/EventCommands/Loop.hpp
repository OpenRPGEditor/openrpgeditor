#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct LoopCommand final : IEventCommand {
  LoopCommand() = default;
  explicit LoopCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~LoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Loop; }
};
