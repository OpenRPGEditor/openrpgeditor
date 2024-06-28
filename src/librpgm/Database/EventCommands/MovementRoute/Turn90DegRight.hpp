#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTurn90DegRightCommand : IEventCommand {
  ~MovementTurn90DegRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Left; }
};