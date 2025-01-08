#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSteppingAnimationOFFCommand final : IMovementRouteStep {
  MovementSteppingAnimationOFFCommand() = default;
  explicit MovementSteppingAnimationOFFCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IMovementRouteStep(indent, parameters) {}
  ~MovementSteppingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Stepping_Animation_OFF; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementSteppingAnimationOFFCommand>(*this); }
};
