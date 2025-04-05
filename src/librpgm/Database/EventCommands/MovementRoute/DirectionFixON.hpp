#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementDirectionFixONCommand final : IMovementRouteStep {
  MovementDirectionFixONCommand() = default;
  explicit MovementDirectionFixONCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IMovementRouteStep(indent, parameters) {}
  ~MovementDirectionFixONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Direction_Fix_ON; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementDirectionFixONCommand>(*this); }
};
