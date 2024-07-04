#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveLowerRightCommand : IMovementRouteStep {
  ~MovementMoveLowerRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Lower_Right; }
};
