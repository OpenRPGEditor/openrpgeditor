#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveTowardPlayerCommand final : IMovementRouteStep {
  MovementMoveTowardPlayerCommand() = default;
  explicit MovementMoveTowardPlayerCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementMoveTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_toward_Player; }
};
