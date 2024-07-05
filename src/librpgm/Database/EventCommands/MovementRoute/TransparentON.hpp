#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTransparentONCommand final : IMovementRouteStep {
  MovementTransparentONCommand() = default;
  explicit MovementTransparentONCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementTransparentONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_ON; }
};
