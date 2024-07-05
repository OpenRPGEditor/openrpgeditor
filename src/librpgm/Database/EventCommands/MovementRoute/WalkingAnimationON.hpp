#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementWalkingAnimationONCommand final : IMovementRouteStep {
  MovementWalkingAnimationONCommand() = default;
  explicit MovementWalkingAnimationONCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementWalkingAnimationONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Walking_Animation_ON; }
};
