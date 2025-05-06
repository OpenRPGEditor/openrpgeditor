#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnRightCommand final : IMovementRouteStep {
  MovementTurnRightCommand() = default;
  explicit MovementTurnRightCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementTurnRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Right; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementTurnRightCommand>(*this); }
};