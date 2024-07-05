#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSwitchONCommand final : IMovementRouteStep {
  ~MovementSwitchONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_ON; }
  int id;

  void serializeParameters(nlohmann::json& out) override { out.push_back(id); }

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
