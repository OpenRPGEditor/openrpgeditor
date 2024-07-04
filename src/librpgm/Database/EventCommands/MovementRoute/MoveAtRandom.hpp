#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveAtRandomCommand : IMovementRouteStep {
  ~MovementMoveAtRandomCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_at_Random; }
};
