#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveRightCommand final : IMovementRouteStep {
  MovementMoveRightCommand() = default;
  explicit MovementMoveRightCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementMoveRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Right; }
};
