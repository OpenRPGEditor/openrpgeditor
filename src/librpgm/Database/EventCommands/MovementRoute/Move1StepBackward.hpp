#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMove1StepBackwardCommand final : IMovementRouteStep {
  MovementMove1StepBackwardCommand() = default;
  explicit MovementMove1StepBackwardCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementMove1StepBackwardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Backward; }
};
