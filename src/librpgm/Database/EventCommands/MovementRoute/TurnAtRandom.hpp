#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementTurnAtRandomCommand final : IMovementRouteStep {
  MovementTurnAtRandomCommand() = default;
  explicit MovementTurnAtRandomCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementTurnAtRandomCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_at_Random; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementTurnAtRandomCommand>(*this); }
};