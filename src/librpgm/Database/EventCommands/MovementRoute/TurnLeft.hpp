#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnLeftCommand : IMovementRouteStep {
  ~MovementTurnLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Left; }
};
