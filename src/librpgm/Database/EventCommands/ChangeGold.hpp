#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeGoldCommand final : IEventCommand {
  ChangeGoldCommand() = default;
  explicit ChangeGoldCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeGoldCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Gold; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  QuantityChangeOp operation = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource operandSource = QuantityChangeSource::Constant;
  int operand = 1;
};
