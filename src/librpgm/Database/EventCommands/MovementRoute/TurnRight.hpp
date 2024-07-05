#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnRightCommand final : IMovementRouteStep {
  MovementTurnRightCommand() = default;
  explicit MovementTurnRightCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementTurnRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Right; }
};