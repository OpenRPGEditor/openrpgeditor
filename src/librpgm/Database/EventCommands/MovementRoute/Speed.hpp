#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSpeedCommand final : IMovementRouteStep {
  ~MovementSpeedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Speed; }
  int speed;

  void serializeParameters(nlohmann::json& out) override { out.push_back(speed); }

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
