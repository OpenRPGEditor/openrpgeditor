#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurn180DegCommand final : IMovementRouteStep {
  ~MovementTurn180DegCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_180_deg; }
};
