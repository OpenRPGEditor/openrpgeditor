#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurn90DegLeftOrRightCommand final : IMovementRouteStep {
  ~MovementTurn90DegLeftOrRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Left_or_Right; }
};
