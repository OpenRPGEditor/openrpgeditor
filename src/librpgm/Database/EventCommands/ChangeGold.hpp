#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeGoldCommand final : IEventCommand {
  ChangeGoldCommand() = default;
  explicit ChangeGoldCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeGoldCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Gold; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeGoldCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return operandSource == QuantityChangeSource::Variable && operand == targetId;
    }
    return false;
  };
  QuantityChangeOp operation = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource operandSource = QuantityChangeSource::Constant;
  int operand = 1;
};
