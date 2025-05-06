#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementDirectionFixOFFCommand final : IMovementRouteStep {
  MovementDirectionFixOFFCommand() = default;
  explicit MovementDirectionFixOFFCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementDirectionFixOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Direction_Fix_OFF; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementDirectionFixOFFCommand>(*this); }
};
