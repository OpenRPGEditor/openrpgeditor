#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveDownCommand final : IMovementRouteStep {
  ~MovementMoveDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Down; }
};
