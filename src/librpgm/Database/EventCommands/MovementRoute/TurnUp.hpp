#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnUpCommand : IMovementRouteStep {
  ~MovementTurnUpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Up; }
};
