#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTransparentOFFCommand final : IMovementRouteStep {
  MovementTransparentOFFCommand() = default;
  explicit MovementTransparentOFFCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementTransparentOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_OFF; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementTransparentOFFCommand>(*this); }
};