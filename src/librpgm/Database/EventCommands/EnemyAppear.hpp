#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EnemyAppearCommand final : IEventCommand {
  ~EnemyAppearCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Appear; }
  int enemy;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};