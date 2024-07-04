#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTransparentONCommand : IMovementRouteStep {
  ~MovementTransparentONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_ON; }
};
