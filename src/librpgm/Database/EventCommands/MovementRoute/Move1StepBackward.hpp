#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMove1StepBackwardCommand : IMovementRouteStep {
  ~MovementMove1StepBackwardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Backward; }
};
