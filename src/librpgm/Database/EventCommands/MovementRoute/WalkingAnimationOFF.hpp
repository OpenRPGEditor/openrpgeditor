#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementWalkingAnimationOFFCommand final : IMovementRouteStep {
  ~MovementWalkingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Walking_Animation_OFF; }
};
