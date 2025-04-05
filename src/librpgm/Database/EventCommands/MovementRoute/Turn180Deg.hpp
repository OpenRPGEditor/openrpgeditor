#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurn180DegCommand final : IMovementRouteStep {
  MovementTurn180DegCommand() = default;
  explicit MovementTurn180DegCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementTurn180DegCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_180_deg; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementTurn180DegCommand>(*this); }
};
