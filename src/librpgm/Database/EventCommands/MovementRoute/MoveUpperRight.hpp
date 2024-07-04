#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveUpperRightCommand : IMovementRouteStep {
  ~MovementMoveUpperRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Upper_Right; }
};