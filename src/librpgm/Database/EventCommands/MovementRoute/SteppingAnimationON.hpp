#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSteppingAnimationONCommand final : IMovementRouteStep {
  MovementSteppingAnimationONCommand() = default;
  explicit MovementSteppingAnimationONCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementSteppingAnimationONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Stepping_Animation_ON; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementSteppingAnimationONCommand>(*this); }
};
