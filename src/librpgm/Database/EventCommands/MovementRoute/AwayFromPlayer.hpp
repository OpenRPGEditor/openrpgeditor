#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementAwayFromPlayerCommand final : IMovementRouteStep {
  MovementAwayFromPlayerCommand() = default;
  explicit MovementAwayFromPlayerCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementAwayFromPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_away_from_Player; }
};
