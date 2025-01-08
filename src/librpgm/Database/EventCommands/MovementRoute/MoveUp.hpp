#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveUpCommand final : IMovementRouteStep {
  MovementMoveUpCommand() = default;
  explicit MovementMoveUpCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IMovementRouteStep(indent, parameters) {}
  ~MovementMoveUpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Up; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementMoveUpCommand>(*this); }
};
