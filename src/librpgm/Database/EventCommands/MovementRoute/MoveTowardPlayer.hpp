#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveTowardPlayerCommand final : IMovementRouteStep {
  ~MovementMoveTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_toward_Player; }
};
