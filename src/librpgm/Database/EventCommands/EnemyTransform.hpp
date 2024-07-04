#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EnemyTransformCommand : IEventCommand {
  ~EnemyTransformCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Transform; }
  int enemy;
  int transform;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};