#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveLeftCommand final : IMovementRouteStep {
  MovementMoveLeftCommand() = default;
  explicit MovementMoveLeftCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementMoveLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Left; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementMoveLeftCommand>(*this); }
};
