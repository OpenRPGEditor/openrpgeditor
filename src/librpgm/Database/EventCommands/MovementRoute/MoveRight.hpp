#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveRightCommand final : IMovementRouteStep {
  ~MovementMoveRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Right; }
};
