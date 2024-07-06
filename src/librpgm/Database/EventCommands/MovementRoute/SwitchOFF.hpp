#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSwitchOFFCommand final : IMovementRouteStep {
  MovementSwitchOFFCommand() = default;
  explicit MovementSwitchOFFCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~MovementSwitchOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_OFF; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int id;
};
