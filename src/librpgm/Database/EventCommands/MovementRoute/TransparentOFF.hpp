#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTransparentOFFCommand final : IMovementRouteStep {
  ~MovementTransparentOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_OFF; }
};