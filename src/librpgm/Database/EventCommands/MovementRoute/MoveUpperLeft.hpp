#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveUpperLeftCommand final : IMovementRouteStep {
  MovementMoveUpperLeftCommand() = default;
  explicit MovementMoveUpperLeftCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementMoveUpperLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Upper_Left; }
};
