#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyTPCommand final : IEventCommand {
  ChangeEnemyTPCommand() = default;
  explicit ChangeEnemyTPCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeEnemyTPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_TP; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  int enemy;
  QuantityChangeOp enemyOp;
  QuantityChangeSource quantitySource;
  int quantity;
};
