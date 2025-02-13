#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyMPCommand final : IEventCommand {
  ChangeEnemyMPCommand() = default;
  explicit ChangeEnemyMPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeEnemyMPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_MP; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeEnemyMPCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return quantitySource == QuantityChangeSource::Variable && quantity == targetId;
    }
    if (type == SearchType::Enemy) {
      return targetId == enemy;
    }
    return false;
  };
  int enemy{-1};
  QuantityChangeOp enemyOp = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource quantitySource = QuantityChangeSource::Constant;
  int quantity{1};
};
