#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct BreakLoopCommand final : IEventCommand {
  BreakLoopCommand() = default;
  explicit BreakLoopCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~BreakLoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Break_Loop; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<BreakLoopCommand>(*this); }
};