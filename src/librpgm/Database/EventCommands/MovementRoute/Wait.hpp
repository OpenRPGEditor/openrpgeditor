#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"

#include <format>

struct MovementWaitCommand : IMovementRouteStep {
  int duration;
  ~MovementWaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait_del_Movement; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
