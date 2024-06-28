#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTurn180DegCommand : IEventCommand {
  ~MovementTurn180DegCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_180_deg; }
};
