#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveAwayFromPlayerCommand final : IMovementRouteStep {
  ~MovementMoveAwayFromPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_away_from_Player; }
};
