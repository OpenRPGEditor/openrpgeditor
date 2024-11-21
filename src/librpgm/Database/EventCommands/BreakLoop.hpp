#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct BreakLoopCommand final : IEventCommand {
  BreakLoopCommand() = default;
  explicit BreakLoopCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IEventCommand(indent, parameters) {}
  ~BreakLoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Break_Loop; }
};