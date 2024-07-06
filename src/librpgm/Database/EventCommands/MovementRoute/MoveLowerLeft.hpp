#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveLowerLeftCommand final : IMovementRouteStep {
  MovementMoveLowerLeftCommand() = default;
  explicit MovementMoveLowerLeftCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementMoveLowerLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Lower_Left; }
};
