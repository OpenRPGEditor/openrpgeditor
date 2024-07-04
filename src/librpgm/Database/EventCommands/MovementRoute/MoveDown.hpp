#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveDownCommand : IMovementRouteStep {
  ~MovementMoveDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Down; }
};
