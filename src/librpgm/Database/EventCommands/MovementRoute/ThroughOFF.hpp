#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementThroughOFFCommand final : IMovementRouteStep {
  ~MovementThroughOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Through_OFF; }
};
