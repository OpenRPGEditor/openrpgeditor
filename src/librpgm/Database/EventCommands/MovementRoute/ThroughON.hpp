#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementThroughONCommand final : IMovementRouteStep {
  MovementThroughONCommand() = default;
  explicit MovementThroughONCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementThroughONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Through_ON; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementThroughONCommand>(*this); }
};
