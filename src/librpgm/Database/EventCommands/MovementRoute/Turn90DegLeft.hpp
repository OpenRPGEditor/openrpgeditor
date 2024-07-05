#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurn90DegLeftCommand final : IMovementRouteStep {
  MovementTurn90DegLeftCommand() = default;
  explicit MovementTurn90DegLeftCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementTurn90DegLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Right; }
};
