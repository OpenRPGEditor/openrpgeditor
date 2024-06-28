#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTurn90DegLeftCommand : IEventCommand {
  ~MovementTurn90DegLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Right; }
};
