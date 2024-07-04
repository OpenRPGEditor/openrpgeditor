#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMove1StepFowardCommand : IMovementRouteStep {
  ~MovementMove1StepFowardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Forward; }
};
