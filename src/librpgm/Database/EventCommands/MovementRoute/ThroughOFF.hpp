#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementThroughOFFCommand final : IMovementRouteStep {
  MovementThroughOFFCommand() = default;
  explicit MovementThroughOFFCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementThroughOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Through_OFF; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementThroughOFFCommand>(*this); }
};
