#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementAwayFromPlayerCommand final : IMovementRouteStep {
  ~MovementAwayFromPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_away_from_Player; }
};
