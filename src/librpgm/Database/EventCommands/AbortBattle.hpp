#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct AbortBattleCommand final : IEventCommand {
  AbortBattleCommand() = default;
  explicit AbortBattleCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~AbortBattleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Abort_Battle; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<AbortBattleCommand>(*this); }
};