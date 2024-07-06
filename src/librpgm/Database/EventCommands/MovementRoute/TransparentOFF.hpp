#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTransparentOFFCommand final : IMovementRouteStep {
  MovementTransparentOFFCommand() = default;
  explicit MovementTransparentOFFCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementTransparentOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_OFF; }
};