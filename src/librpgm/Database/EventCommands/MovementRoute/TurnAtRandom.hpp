#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnAtRandomCommand : IMovementRouteStep {
  ~MovementTurnAtRandomCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_at_Random; }
};