#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyHPCommand final : IEventCommand {
  ChangeEnemyHPCommand() = default;
  explicit ChangeEnemyHPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeEnemyHPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_HP; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int enemy{-1};
  QuantityChangeOp enemyOp = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource quantitySource = QuantityChangeSource::Constant;
  int quantity{1};
  bool allowKnockOut{false};
};