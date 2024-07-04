#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSteppingAnimationONCommand : IMovementRouteStep {
  ~MovementSteppingAnimationONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};
