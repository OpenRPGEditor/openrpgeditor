#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementDirectionFixONCommand final : IMovementRouteStep {
  ~MovementDirectionFixONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Direction_Fix_ON; }
};
