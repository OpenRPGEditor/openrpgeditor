#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementDirectionFixOFFCommand final : IMovementRouteStep {
  MovementDirectionFixOFFCommand() = default;
  explicit MovementDirectionFixOFFCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementDirectionFixOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Direction_Fix_OFF; }
};
