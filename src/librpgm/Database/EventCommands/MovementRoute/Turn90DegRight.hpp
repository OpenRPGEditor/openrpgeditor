#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurn90DegRightCommand final : IMovementRouteStep {
  MovementTurn90DegRightCommand() = default;
  explicit MovementTurn90DegRightCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementTurn90DegRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Right; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementTurn90DegRightCommand>(*this); }
};