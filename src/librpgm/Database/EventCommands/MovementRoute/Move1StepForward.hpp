#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMove1StepFowardCommand final : IMovementRouteStep {
  MovementMove1StepFowardCommand() = default;
  explicit MovementMove1StepFowardCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementMove1StepFowardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Forward; }
};
