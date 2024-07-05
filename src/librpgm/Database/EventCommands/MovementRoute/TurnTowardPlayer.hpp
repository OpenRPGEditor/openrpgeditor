#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnTowardPlayerCommand final : IMovementRouteStep {
  ~MovementTurnTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_toward_Player; }
};
