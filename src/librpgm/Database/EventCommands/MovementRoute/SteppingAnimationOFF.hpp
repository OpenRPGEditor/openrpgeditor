#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSteppingAnimationOFFCommand final : IMovementRouteStep {
  ~MovementSteppingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Stepping_Animation_OFF; }
};
