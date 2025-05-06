#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementWalkingAnimationONCommand final : IMovementRouteStep {
  MovementWalkingAnimationONCommand() = default;
  explicit MovementWalkingAnimationONCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementWalkingAnimationONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Walking_Animation_ON; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementWalkingAnimationONCommand>(*this); }
};
