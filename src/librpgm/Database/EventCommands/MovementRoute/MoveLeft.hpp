#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveLeftCommand final : IMovementRouteStep {
  ~MovementMoveLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Left; }
};
