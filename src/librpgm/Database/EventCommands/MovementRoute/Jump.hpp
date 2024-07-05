#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementJumpCommand final : IMovementRouteStep {
  MovementJumpCommand() = default;
  explicit MovementJumpCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~MovementJumpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump; }
  int x;
  int y;

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(x);
    out.push_back(y);
  }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
