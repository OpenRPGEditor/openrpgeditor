#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementJumpCommand final : IMovementRouteStep {
  MovementJumpCommand() = default;
  explicit MovementJumpCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementJumpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump; }
  int x{0};
  int y{0};

  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
