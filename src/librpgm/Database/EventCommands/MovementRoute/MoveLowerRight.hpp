#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveLowerRightCommand final : IMovementRouteStep {
  MovementMoveLowerRightCommand() = default;
  explicit MovementMoveLowerRightCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementMoveLowerRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Lower_Right; }
};
