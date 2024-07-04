#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveLowerLeftCommand : IMovementRouteStep {
  ~MovementMoveLowerLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Lower_Left; }
};
