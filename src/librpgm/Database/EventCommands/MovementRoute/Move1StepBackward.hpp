#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMove1StepBackwardCommand final : IMovementRouteStep {
  MovementMove1StepBackwardCommand() = default;
  explicit MovementMove1StepBackwardCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementMove1StepBackwardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Backward; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementMove1StepBackwardCommand>(*this); }
};
