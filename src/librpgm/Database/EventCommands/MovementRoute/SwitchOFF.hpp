#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSwitchOFFCommand final : IMovementRouteStep {
  ~MovementSwitchOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_OFF; }
  int id;

  void serializeParameters(nlohmann::json& out) override { out.push_back(id); }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
