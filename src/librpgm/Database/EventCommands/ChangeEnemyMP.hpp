#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyMPCommand final : IEventCommand {
  ChangeEnemyMPCommand() = default;
  explicit ChangeEnemyMPCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeEnemyMPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_MP; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int enemy{-1};
  QuantityChangeOp enemyOp = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource quantitySource = QuantityChangeSource::Constant;
  int quantity{1};
};
