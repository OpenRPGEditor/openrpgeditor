#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveTowardPlayerCommand final : IMovementRouteStep {
  MovementMoveTowardPlayerCommand() = default;
  explicit MovementMoveTowardPlayerCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementMoveTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_toward_Player; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementMoveTowardPlayerCommand>(*this); }
};
