#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSpeedCommand final : IMovementRouteStep {
  MovementSpeedCommand() = default;
  explicit MovementSpeedCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementSpeedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Speed; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementSpeedCommand>(*this); }

  int speed{4};
};
