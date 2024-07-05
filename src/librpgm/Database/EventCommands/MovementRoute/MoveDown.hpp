#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveDownCommand final : IMovementRouteStep {
  MovementMoveDownCommand() = default;
  explicit MovementMoveDownCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementMoveDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Down; }
};
