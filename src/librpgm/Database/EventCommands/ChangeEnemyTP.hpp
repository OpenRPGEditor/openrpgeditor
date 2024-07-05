#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyTPCommand final : IEventCommand {
  ~ChangeEnemyTPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_TP; }
  int enemy;
  QuantityChangeOp enemyOp;
  QuantityChangeSource quantitySource;
  int quantity;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
