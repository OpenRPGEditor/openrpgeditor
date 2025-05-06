#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveRightCommand final : IMovementRouteStep {
  MovementMoveRightCommand() = default;
  explicit MovementMoveRightCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementMoveRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Right; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementMoveRightCommand>(*this); }
};
