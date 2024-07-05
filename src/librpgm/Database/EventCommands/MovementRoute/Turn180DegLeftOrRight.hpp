#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurn90DegLeftOrRightCommand final : IMovementRouteStep {
  MovementTurn90DegLeftOrRightCommand() = default;
  explicit MovementTurn90DegLeftOrRightCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementTurn90DegLeftOrRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Left_or_Right; }
};
