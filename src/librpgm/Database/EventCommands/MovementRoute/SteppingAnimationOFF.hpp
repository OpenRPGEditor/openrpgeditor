#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSteppingAnimationOFFCommand : IMovementRouteStep {
  ~MovementSteppingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};
