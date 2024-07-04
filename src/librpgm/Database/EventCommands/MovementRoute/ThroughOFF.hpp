#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementThroughOFFCommand : IMovementRouteStep {
  ~MovementThroughOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Through_OFF; }
};
