#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementWalkingAnimationOFFCommand : IMovementRouteStep {
  ~MovementWalkingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};
