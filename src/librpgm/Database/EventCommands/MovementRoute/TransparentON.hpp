#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTransparentONCommand final : IMovementRouteStep {
  MovementTransparentONCommand() = default;
  explicit MovementTransparentONCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementTransparentONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_ON; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementTransparentONCommand>(*this); }
};
