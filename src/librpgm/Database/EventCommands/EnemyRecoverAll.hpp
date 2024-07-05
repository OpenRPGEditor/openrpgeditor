#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EnemyRecoverAllCommand final : IEventCommand {
  ~EnemyRecoverAllCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Recover_All; }
  int troop;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};