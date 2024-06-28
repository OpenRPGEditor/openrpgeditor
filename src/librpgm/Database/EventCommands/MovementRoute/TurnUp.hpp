#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTurnUpCommand : IEventCommand {
  ~MovementTurnUpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Up; }
};
