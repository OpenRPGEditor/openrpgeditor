#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnTowardPlayerCommand final : IMovementRouteStep {
  MovementTurnTowardPlayerCommand() = default;
  explicit MovementTurnTowardPlayerCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementTurnTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_toward_Player; }
};
