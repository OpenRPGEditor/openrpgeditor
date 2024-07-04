#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveUpCommand : IMovementRouteStep {
  ~MovementMoveUpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Up; }
};
