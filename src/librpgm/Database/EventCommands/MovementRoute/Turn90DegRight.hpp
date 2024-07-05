#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurn90DegRightCommand final : IMovementRouteStep {
  ~MovementTurn90DegRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Left; }
};