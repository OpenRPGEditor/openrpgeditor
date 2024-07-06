#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSteppingAnimationOFFCommand final : IMovementRouteStep {
  MovementSteppingAnimationOFFCommand() = default;
  explicit MovementSteppingAnimationOFFCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementSteppingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Stepping_Animation_OFF; }
};
