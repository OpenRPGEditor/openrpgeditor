#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"

#include <format>

struct MovementWaitCommand final : IMovementRouteStep {
  MovementWaitCommand() = default;
  explicit MovementWaitCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~MovementWaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait_del_Movement; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int duration;
};
