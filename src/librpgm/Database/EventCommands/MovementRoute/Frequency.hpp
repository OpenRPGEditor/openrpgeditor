#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementFrequencyCommand final : IMovementRouteStep {
  MovementFrequencyCommand() = default;
  explicit MovementFrequencyCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~MovementFrequencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int frequency{2};
};
