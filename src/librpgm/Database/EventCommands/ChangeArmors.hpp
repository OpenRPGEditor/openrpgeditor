#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeArmorsCommand final : IEventCommand {
  ChangeArmorsCommand() = default;
  explicit ChangeArmorsCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeArmorsCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Armors; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeArmorsCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return operandSource == QuantityChangeSource::Variable && operand == targetId;
    }
    if (type == SearchType::Armors) {
      return targetId == item;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (type == SearchType::Variable) {
        if (operandSource == QuantityChangeSource::Variable && operand == targetId) {
          operand = newId;
        }
      } else if (type == SearchType::Armors) {
        item = newId;
      }
      return true;
    }
    return false;
  }
  int item{1};

  QuantityChangeOp operation = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource operandSource = QuantityChangeSource::Constant;
  int operand{1};
  bool includeEquipment{false};
};