#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveUpperRightCommand final : IMovementRouteStep {
  ~MovementMoveUpperRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Upper_Right; }
};