#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyTPCommand final : IEventCommand {
  ChangeEnemyTPCommand() = default;
  explicit ChangeEnemyTPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeEnemyTPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_TP; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeEnemyTPCommand>(*this); }

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
      if (type == SearchType::Enemy) {
        enemy = newId;
      }
      if (quantitySource == QuantityChangeSource::Variable && quantity == targetId) {
        quantity = newId;
      }

      return true;
    }
    return false;
  }

  int enemy{-1};
  QuantityChangeOp enemyOp = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource quantitySource = QuantityChangeSource::Constant;
  int quantity{1};
};
