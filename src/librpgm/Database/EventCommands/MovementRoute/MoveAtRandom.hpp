#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveAtRandomCommand final : IMovementRouteStep {
  MovementMoveAtRandomCommand() = default;
  explicit MovementMoveAtRandomCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementMoveAtRandomCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_at_Random; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementMoveAtRandomCommand>(*this); }
};
