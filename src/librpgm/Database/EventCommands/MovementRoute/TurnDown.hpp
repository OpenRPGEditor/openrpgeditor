#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTurnDownCommand : IEventCommand {
  ~MovementTurnDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Down; }
};
