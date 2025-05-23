#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnLeftCommand final : IMovementRouteStep {
  MovementTurnLeftCommand() = default;
  explicit MovementTurnLeftCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementTurnLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Left; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementTurnLeftCommand>(*this); }
};
