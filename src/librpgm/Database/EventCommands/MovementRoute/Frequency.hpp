#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementFrequencyCommand : IMovementRouteStep {
  ~MovementFrequencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
  int frequency;
  void serializeParameters(nlohmann::json& out) override { out.push_back(frequency); }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
