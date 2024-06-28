#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementAwayFromPlayerCommand : IEventCommand {
  ~MovementAwayFromPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_away_from_Player; }
};
