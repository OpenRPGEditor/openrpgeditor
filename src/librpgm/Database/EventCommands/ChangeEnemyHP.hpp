#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyHPCommand final : IEventCommand {
  ChangeEnemyHPCommand() = default;
  explicit ChangeEnemyHPCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeEnemyHPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_HP; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int enemy;
  QuantityChangeOp enemyOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool allowKnockOut;
};