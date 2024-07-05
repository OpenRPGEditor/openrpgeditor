#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSteppingAnimationONCommand final : IMovementRouteStep {
  ~MovementSteppingAnimationONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Stepping_Animation_ON; }
};
