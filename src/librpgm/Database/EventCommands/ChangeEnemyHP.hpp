#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyHPCommand : IEventCommand {
  ~ChangeEnemyHPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_HP; }
  int enemy;
  QuantityChangeOp enemyOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool allowKnockOut;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};