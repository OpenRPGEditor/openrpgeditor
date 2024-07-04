#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveUpperLeftCommand : IMovementRouteStep {
  ~MovementMoveUpperLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Upper_Left; }
};
