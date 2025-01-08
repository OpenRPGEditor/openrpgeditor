#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveUpperRightCommand final : IMovementRouteStep {
  MovementMoveUpperRightCommand() = default;
  explicit MovementMoveUpperRightCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IMovementRouteStep(indent, parameters) {}
  ~MovementMoveUpperRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Upper_Right; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementMoveUpperRightCommand>(*this); }
};