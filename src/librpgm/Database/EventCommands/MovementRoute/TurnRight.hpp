#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnRightCommand final : IMovementRouteStep {
  ~MovementTurnRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Right; }
};