#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnDownCommand final : IMovementRouteStep {
  MovementTurnDownCommand() = default;
  explicit MovementTurnDownCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementTurnDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Down; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementTurnDownCommand>(*this); }
};
