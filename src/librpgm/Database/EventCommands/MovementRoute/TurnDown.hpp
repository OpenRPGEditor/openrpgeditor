#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnDownCommand final : IMovementRouteStep {
  ~MovementTurnDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Down; }
};
