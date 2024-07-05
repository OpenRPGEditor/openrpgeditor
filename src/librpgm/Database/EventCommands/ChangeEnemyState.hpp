#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyStateCommand final : IEventCommand {
  ~ChangeEnemyStateCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_TP; }
  int enemy;
  PartyMemberOperation enemyOp;
  int state;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};