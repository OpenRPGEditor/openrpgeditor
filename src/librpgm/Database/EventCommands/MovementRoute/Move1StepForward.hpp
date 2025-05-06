#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMove1StepForwardCommand final : IMovementRouteStep {
  MovementMove1StepForwardCommand() = default;
  explicit MovementMove1StepForwardCommand(const std::optional<int>& indent)
  : IMovementRouteStep(indent) {}
  ~MovementMove1StepForwardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Forward; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementMove1StepForwardCommand>(*this); }
};
