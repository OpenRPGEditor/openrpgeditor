#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnLeftCommand final : IMovementRouteStep {
  ~MovementTurnLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Left; }
};
