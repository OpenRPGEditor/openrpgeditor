#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTurnLeftCommand : IEventCommand {
  ~MovementTurnLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Left; }
};
