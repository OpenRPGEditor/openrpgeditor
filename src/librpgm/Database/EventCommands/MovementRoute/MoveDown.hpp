#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementMoveDownCommand final : IMovementRouteStep {
  MovementMoveDownCommand() = default;
  explicit MovementMoveDownCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IMovementRouteStep(indent, parameters) {}
  ~MovementMoveDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Down; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementMoveDownCommand>(*this); }
};
