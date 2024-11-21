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

  int speed{4};
};
