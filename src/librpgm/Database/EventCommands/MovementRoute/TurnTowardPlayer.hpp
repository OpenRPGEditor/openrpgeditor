#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTurnTowardPlayerCommand : IEventCommand {
  ~MovementTurnTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_toward_Player; }
};
