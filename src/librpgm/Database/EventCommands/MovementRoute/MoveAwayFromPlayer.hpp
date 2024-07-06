#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveAwayFromPlayerCommand final : IMovementRouteStep {
  MovementMoveAwayFromPlayerCommand() = default;
  explicit MovementMoveAwayFromPlayerCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementMoveAwayFromPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_away_from_Player; }
};
