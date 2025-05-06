#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnUpCommand final : IMovementRouteStep {
  MovementTurnUpCommand() = default;
  explicit MovementTurnUpCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementTurnUpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Up; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementTurnUpCommand>(*this); }
};
