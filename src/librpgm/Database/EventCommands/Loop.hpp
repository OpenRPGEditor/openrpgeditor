#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct LoopCommand final : IEventCommand {
  LoopCommand() = default;
  explicit LoopCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IEventCommand(indent, parameters) {}
  ~LoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Loop; }
};
