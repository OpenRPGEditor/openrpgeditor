#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTurn90DegLeftOrRightCommand : IEventCommand {
  ~MovementTurn90DegLeftOrRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Left_or_Right; }
};
