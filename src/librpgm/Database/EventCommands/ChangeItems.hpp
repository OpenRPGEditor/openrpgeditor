#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeItemsCommand final : IEventCommand {
  ChangeItemsCommand() = default;
  explicit ChangeItemsCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeItemsCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Items; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeItemsCommand>(*this); }

  int item{1};
  QuantityChangeOp operation = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource operandSource = QuantityChangeSource::Constant;
  int operand{1};
};
