#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeItemsCommand final : IEventCommand {
  ChangeItemsCommand() = default;
  explicit ChangeItemsCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeItemsCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Items; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeItemsCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return operandSource == QuantityChangeSource::Variable && operand == targetId;
    }
    if (type == SearchType::Items) {
      return targetId == item;
    }
    return false;
  };

  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (operandSource == QuantityChangeSource::Variable && operand == targetId) {
        operand = newId;
      }
      if (type == SearchType::Items) {
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
};
