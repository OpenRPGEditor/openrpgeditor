#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct AbortBattleCommand : IEventCommand {
  ~AbortBattleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Abort_Battle; }
};