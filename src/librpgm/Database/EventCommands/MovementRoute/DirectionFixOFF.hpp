#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementDirectionFixOFFCommand : IMovementRouteStep {
  ~MovementDirectionFixOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Direction_Fix_OFF; }
};
