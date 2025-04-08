#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyHPCommand final : IEventCommand {
  ChangeEnemyHPCommand() = default;
  explicit ChangeEnemyHPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeEnemyHPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_HP; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeEnemyHPCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return quantitySource == QuantityChangeSource::Variable && quantity == targetId;
    }
    if (type == SearchType::Enemy) {
      return targetId == enemy;
    }
    return false;
  };

  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (quantitySource == QuantityChangeSource::Variable && quantity == targetId) {
        quantity = newId;
      }
      if (type == SearchType::Enemy) {
        enemy = newId;
      }

      return true;
    }
    return false;
  }
  int enemy{-1};
  QuantityChangeOp enemyOp = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource quantitySource = QuantityChangeSource::Constant;
  int quantity{1};
  bool allowKnockOut{false};
};