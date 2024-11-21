#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementWalkingAnimationOFFCommand final : IMovementRouteStep {
  MovementWalkingAnimationOFFCommand() = default;
  explicit MovementWalkingAnimationOFFCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementWalkingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Walking_Animation_OFF; }
};
