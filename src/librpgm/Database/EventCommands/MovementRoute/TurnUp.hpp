#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnUpCommand final : IMovementRouteStep {
  MovementTurnUpCommand() = default;
  explicit MovementTurnUpCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementTurnUpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Up; }
};
