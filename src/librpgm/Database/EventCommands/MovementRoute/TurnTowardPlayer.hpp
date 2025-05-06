#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnTowardPlayerCommand final : IMovementRouteStep {
  MovementTurnTowardPlayerCommand() = default;
  explicit MovementTurnTowardPlayerCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementTurnTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_toward_Player; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementTurnTowardPlayerCommand>(*this); }
};
