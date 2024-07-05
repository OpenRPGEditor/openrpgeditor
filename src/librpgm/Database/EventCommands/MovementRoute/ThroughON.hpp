#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementThroughONCommand final : IMovementRouteStep {
  ~MovementThroughONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Through_ON; }
};
